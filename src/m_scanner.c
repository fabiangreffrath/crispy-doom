//
//  Copyright (c) 2015, Braden "Blzut3" Obrzut
//  Copyright (c) 2024, Roman Fomin
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are
// met:
//     * Redistributions of source code must retain the above copyright
//       notice, this list of conditions and the following disclaimer.
//     * Redistributions in binary form must reproduce the above copyright
//       notice, this list of conditions and the following disclaimer in the
//       documentation and/or other materials provided with the
//       distribution.
//     * The names of its contributors may be used to endorse or promote
//       products derived from this software without specific prior written
//       permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
// ARE DISCLAIMED. IN NO EVENT SHALL COPYRIGHT HOLDER BE LIABLE FOR ANY
// DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
// (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
// SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
// CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
// LIABILITY, OR TORT(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
// OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
// SUCH DAMAGE.

#include "m_scanner.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "doomtype.h"
#include "i_system.h"
#include "m_misc.h"

static const char *const token_names[] = {
    [TK_Identifier] = "Identifier",
    [TK_StringConst] = "String Constant",
    [TK_IntConst] = "Integer Constant",
    [TK_BoolConst] = "Boolean Constant",
    [TK_FloatConst] = "Float Constant",
    [TK_AnnotateStart] = "Annotation Start",
    [TK_AnnotateEnd] = "Annotation End",
    [TK_ScopeResolution] = "Scope Resolution"};

typedef struct
{
    char *string;
    int length;
    int number;
    double decimal;
    char token;

    int tokenline;
    int tokenlinepos;
    int scanpos;
} parserstate_t;

struct scanner_s
{
    parserstate_t state;
    parserstate_t nextstate, prevstate;

    const char *data;
    size_t length;

    int line;
    int linestart;
    int logicalpos;
    int scanpos;

    boolean neednext; // If CheckToken() returns false this will be false.

    const char *scriptname;
};

static void IncrementLine(scanner_t *s)
{
    s->line++;
    s->linestart = s->scanpos;
}

static void CheckForWhitespace(scanner_t *s)
{
    int comment = 0; // 1 = till next new line, 2 = till end block

    while (s->scanpos < s->length)
    {
        char cur = s->data[s->scanpos];
        char next = (s->scanpos + 1 < s->length) ? s->data[s->scanpos + 1] : 0;

        if (comment == 2)
        {
            if (cur != '*' || next != '/')
            {
                if (cur == '\n' || cur == '\r')
                {
                    s->scanpos++;

                    // Do a quick check for Windows style new line
                    if (cur == '\r' && next == '\n')
                    {
                        s->scanpos++;
                    }
                    IncrementLine(s);
                }
                else
                {
                    s->scanpos++;
                }
            }
            else
            {
                comment = 0;
                s->scanpos += 2;
            }
            continue;
        }

        if (cur == ' ' || cur == '\t' || cur == 0)
        {
            s->scanpos++;
        }
        else if (cur == '\n' || cur == '\r')
        {
            s->scanpos++;
            if (comment == 1)
            {
                comment = 0;
            }

            // Do a quick check for Windows style new line
            if (cur == '\r' && next == '\n')
            {
                s->scanpos++;
            }
            IncrementLine(s);
        }
        else if (cur == '/' && comment == 0)
        {
            switch (next)
            {
                case '/':
                    comment = 1;
                    break;
                case '*':
                    comment = 2;
                    break;
                default:
                    return;
            }
            s->scanpos += 2;
        }
        else
        {
            if (comment == 0)
            {
                return;
            }
            else
            {
                s->scanpos++;
            }
        }
    }
}

static void CopyString(parserstate_t *state, const char *string, int length)
{
    if (state->length < length)
    {
        if (state->string)
        {
            free(state->string);
        }
        state->string = malloc(length + 1);
        state->length = length;
    }
    memcpy(state->string, string, length);
    state->string[length] = '\0';
}

static void CopyState(parserstate_t *to, const parserstate_t *from)
{
    if (from->length)
    {
        CopyString(to, from->string, from->length);
    }
    to->number = from->number;
    to->decimal = from->decimal;
    to->token = from->token;
    to->tokenline = from->tokenline;
    to->tokenlinepos = from->tokenlinepos;
    to->scanpos = from->scanpos;
}

static void ExpandState(scanner_t *s)
{
    s->scanpos = s->nextstate.scanpos;
    s->logicalpos = s->scanpos;
    CheckForWhitespace(s);

    CopyState(&s->prevstate, &s->state);
    CopyState(&s->state, &s->nextstate);
}

static void Unescape(char *str)
{
    char *p = str, c;

    while ((c = *p++))
    {
        if (c != '\\')
        {
            *str++ = c;
        }
        else if (*p)
        {
            switch (*p)
            {
                case 'n':
                    *str++ = '\n';
                    break;
                case 'r':
                    *str++ = '\r';
                    break;
                case 't':
                    *str++ = '\t';
                    break;
                case '"':
                    *str++ = '\"';
                    break;
                default:
                    *str++ = *p;
                    break;
            }
            p++;
        }
        else
        {
            /* Trailing backslash */
        }
    }
    *str = 0;
}

boolean SCN_GetNextToken(scanner_t *s, boolean expandstate)
{
    int start;
    int end;
    int integer_base;
    boolean float_has_decimal;
    boolean float_has_exponent;
    boolean string_finished;
    char cur;

    if (!s->neednext)
    {
        s->neednext = true;
        if (expandstate)
        {
            ExpandState(s);
        }
        return true;
    }

    s->nextstate.tokenline = s->line;
    s->nextstate.tokenlinepos = s->scanpos - s->linestart;
    s->nextstate.token = TK_NoToken;
    if (s->scanpos >= s->length)
    {
        if (expandstate)
        {
            ExpandState(s);
        }
        return false;
    }

    start = s->scanpos;
    end = s->scanpos;
    integer_base = 10;
    float_has_decimal = false;
    float_has_exponent = false;
    // Strings are the only things that can have 0 length tokens.
    string_finished = false;

    cur = s->data[s->scanpos++];
    // Determine by first character
    if (cur == '_' || (cur >= 'A' && cur <= 'Z') || (cur >= 'a' && cur <= 'z'))
    {
        s->nextstate.token = TK_Identifier;
    }
    else if (cur >= '0' && cur <= '9')
    {
        if (cur == '0')
        {
            integer_base = 8;
        }
        s->nextstate.token = TK_IntConst;
    }
    else if (cur == '.' && s->scanpos < s->length && s->data[s->scanpos] != '.')
    {
        float_has_decimal = true;
        s->nextstate.token = TK_FloatConst;
    }
    else if (cur == '"')
    {
        end = ++start; // Move the start up one character so we don't have to
                       // trim it later.
        s->nextstate.token = TK_StringConst;
    }
    else
    {
        end = s->scanpos;
        s->nextstate.token = cur;

        // Now check for operator tokens
        if (s->scanpos < s->length)
        {
            char next = s->data[s->scanpos];

            if (cur == ':' && next == ':')
            {
                s->nextstate.token = TK_ScopeResolution;
            }
            else if (cur == '/' && next == '*')
            {
                s->nextstate.token = TK_AnnotateStart;
            }
            else if (cur == '*' && next == '/')
            {
                s->nextstate.token = TK_AnnotateEnd;
            }

            if (s->nextstate.token != cur)
            {
                s->scanpos++;
                end = s->scanpos;
            }
        }
    }

    if (start == end)
    {
        while (s->scanpos < s->length)
        {
            cur = s->data[s->scanpos];
            switch (s->nextstate.token)
            {
                default:
                    break;

                case TK_Identifier:
                    if (cur != '_' && (cur < 'A' || cur > 'Z') &&
                        (cur < 'a' || cur > 'z') && (cur < '0' || cur > '9'))
                    {
                        end = s->scanpos;
                    }
                    break;

                case TK_IntConst:
                    if (cur == '.' || (s->scanpos - 1 != start && cur == 'e'))
                    {
                        s->nextstate.token = TK_FloatConst;
                    }
                    else if ((cur == 'x' || cur == 'X') &&
                             s->scanpos - 1 == start)
                    {
                        integer_base = 16;
                        break;
                    }
                    else
                    {
                        switch (integer_base)
                        {
                            default:
                                if (cur < '0' || cur > '9')
                                {
                                    end = s->scanpos;
                                }
                                break;
                            case 8:
                                if (cur < '0' || cur > '7')
                                {
                                    end = s->scanpos;
                                }
                                break;
                            case 16:
                                if ((cur < '0' || cur > '9') &&
                                    (cur < 'A' || cur > 'F') &&
                                    (cur < 'a' || cur > 'f'))
                                {
                                    end = s->scanpos;
                                }
                                break;
                        }
                        break;
                    }

                case TK_FloatConst:
                    if (cur < '0' || cur > '9')
                    {
                        if (!float_has_decimal && cur == '.')
                        {
                            float_has_decimal = true;
                            break;
                        }
                        else if (!float_has_exponent && cur == 'e')
                        {
                            float_has_decimal = true;
                            float_has_exponent = true;
                            if (s->scanpos + 1 < s->length)
                            {
                                char next = s->data[s->scanpos + 1];
                                if ((next < '0' || next > '9') && next != '+' &&
                                    next != '-')
                                {
                                    end = s->scanpos;
                                }
                                else
                                {
                                    s->scanpos++;
                                }
                            }
                            break;
                        }
                        end = s->scanpos;
                    }
                    break;

                case TK_StringConst:
                    if (cur == '"')
                    {
                        string_finished = true;
                        end = s->scanpos;
                        s->scanpos++;
                    }
                    else if (cur == '\\')
                    {
                        s->scanpos++; // Will add two since the loop
                                      // automatically adds one
                    }
                    break;
            }
            if (start == end && !string_finished)
            {
                s->scanpos++;
            }
            else
            {
                break;
            }
        }
        // Handle small tokens at the end of a file.
        if (s->scanpos == s->length && !string_finished)
        {
            end = s->scanpos;
        }
    }

    s->nextstate.scanpos = s->scanpos;
    if (end - start > 0 || string_finished)
    {
        CopyString(&s->nextstate, s->data + start, end - start);
        if (s->nextstate.token == TK_FloatConst)
        {
            if (float_has_decimal && strlen(s->nextstate.string) == 1)
            {
                // Don't treat a lone '.' as a decimal.
                s->nextstate.token = '.';
            }
            else
            {
                s->nextstate.decimal = atof(s->nextstate.string);
                s->nextstate.number = (int) s->nextstate.decimal;
            }
        }
        else if (s->nextstate.token == TK_IntConst)
        {
            s->nextstate.number =
                strtol(s->nextstate.string, NULL, integer_base);
            s->nextstate.decimal = (double) s->nextstate.number;
        }
        else if (s->nextstate.token == TK_Identifier)
        {
            // Check for a boolean constant.
            if (!strcasecmp(s->nextstate.string, "true"))
            {
                s->nextstate.token = TK_BoolConst;
                s->nextstate.number = true;
            }
            else if (!strcasecmp(s->nextstate.string, "false"))
            {
                s->nextstate.token = TK_BoolConst;
                s->nextstate.number = false;
            }
        }
        else if (s->nextstate.token == TK_StringConst)
        {
            Unescape(s->nextstate.string);
        }
        if (expandstate)
        {
            ExpandState(s);
        }
        return true;
    }
    s->nextstate.token = TK_NoToken;
    if (expandstate)
    {
        ExpandState(s);
    }
    return false;
}

// Skips all Tokens in current line and parses the first token on the next
// line.
void SCN_GetNextLineToken(scanner_t *s)
{
    int line = s->line;
    while (SCN_GetNextToken(s, true) && s->line == line)
        ;
}

boolean SCN_CheckToken(scanner_t *s, char token)
{
    if (s->neednext)
    {
        if (!SCN_GetNextToken(s, false))
        {
            return false;
        }
    }

    if (s->nextstate.token == token
        // An int can also be a float.
        || (s->nextstate.token == TK_IntConst && token == TK_FloatConst))
    {
        s->neednext = true;
        ExpandState(s);
        return true;
    }
    s->neednext = false;
    return false;
}

void SCN_Error(scanner_t *s, const char *msg, ...)
{
    char buffer[1024];
    va_list args;
    va_start(args, msg);
    M_vsnprintf(buffer, sizeof(buffer), msg, args);
    va_end(args);

    I_Error("%s(%d:%d): %s", s->scriptname, s->state.tokenline,
            s->state.tokenlinepos + 1, buffer);
}

void SCN_MustGetToken(scanner_t *s, char token)
{
    if (SCN_CheckToken(s, token))
    {
        return;
    }

    ExpandState(s);
    if (s->state.token == TK_NoToken)
    {
        SCN_Error(s, "Unexpected end of script.");
    }
    else if (token < TK_NumSpecialTokens &&
             s->state.token < TK_NumSpecialTokens)
    {
        SCN_Error(s, "Expected '%s' but got '%s' instead.",
                 token_names[(int) token], token_names[(int) s->state.token]);
    }
    else if (token < TK_NumSpecialTokens &&
             s->state.token >= TK_NumSpecialTokens)
    {
        SCN_Error(s, "Expected '%s' but got '%c' instead.",
                 token_names[(int) token], s->state.token);
    }
    else if (token >= TK_NumSpecialTokens &&
             s->state.token < TK_NumSpecialTokens)
    {
        SCN_Error(s, "Expected '%c' but got '%s' instead.", token,
                 token_names[(int) s->state.token]);
    }
    else
    {
        SCN_Error(s, "Expected '%c' but got '%c' instead.", token,
                 s->state.token);
    }
}

void SCN_Rewind(scanner_t *s) // Only can rewind one step.
{
    s->neednext = false;

    CopyState(&s->nextstate, &s->state);
    CopyState(&s->state, &s->prevstate);

    s->line = s->prevstate.tokenline;
    s->logicalpos = s->prevstate.tokenlinepos;
    s->scanpos = s->prevstate.scanpos;

    CheckForWhitespace(s);
}

boolean SCN_SameLine(scanner_t *s)
{
    return (s->state.tokenline == s->line);
}

boolean SCN_CheckStringOrIdent(scanner_t *s)
{
    return (SCN_CheckToken(s, TK_StringConst) ||
            SCN_CheckToken(s, TK_Identifier));
}

void SCN_MustGetStringOrIdent(scanner_t *s)
{
    if (!SCN_CheckStringOrIdent(s))
    {
        SCN_Error(s, "expected string constant or identifier");
    }
}

boolean SCN_GetNextRawString(scanner_t *s, boolean expandstate)
{
    int start;
    int end;
    boolean quoted;
    boolean string_finished;

    if (!s->neednext)
    {
        s->neednext = true;
        if (expandstate)
        {
            ExpandState(s);
        }
        return true;
    }

    s->nextstate.tokenline = s->line;
    s->nextstate.tokenlinepos = s->scanpos - s->linestart;
    s->nextstate.token = TK_NoToken;

    CheckForWhitespace(s);

    if (s->scanpos >= s->length)
    {
        if (expandstate)
        {
            ExpandState(s);
        }
        return false;
    }

    start = s->scanpos;
    end = s->scanpos;
    quoted = false;
    string_finished = false;

    // Check if string starts with a quote
    if (s->data[s->scanpos] == '"')
    {
        quoted = true;
        start = ++s->scanpos; // Skip opening quote
    }

    while (s->scanpos < s->length)
    {
        char cur = s->data[s->scanpos];
        if (quoted)
        {
            if (cur == '"')
            {
                string_finished = true;
                end = s->scanpos;
                s->scanpos++; // Skip closing quote
                break;
            }
            else if (cur == '\\')
            {
                s->scanpos++; // Skip escape character
            }
            else if (cur == '\n' || cur == '\r')
            {
                // Unterminated string - treat as error or end at line
                end = s->scanpos;
                break;
            }
        }
        else
        {
            if (cur == ' ' || cur == '\t' || cur == '\n' || cur == '\r' ||
                cur == 0)
            {
                end = s->scanpos;
                break;
            }
        }
        s->scanpos++;
    }

    if (s->scanpos == s->length && !string_finished)
    {
        end = s->scanpos;
    }

    s->nextstate.scanpos = s->scanpos;

    if (end - start > 0)
    {
        CopyString(&s->nextstate, s->data + start, end - start);
        if (quoted)
        {
            Unescape(s->nextstate.string);
            s->nextstate.token = TK_StringConst;
        }
        else
        {
            s->nextstate.token = s->data[start];
        }
        if (expandstate)
        {
            ExpandState(s);
        }
        return true;
    }

    if (expandstate)
    {
        ExpandState(s);
    }
    return false;
}

boolean SCN_CheckRawToken(scanner_t *s, char token)
{
    if (s->neednext)
    {
        if (!SCN_GetNextRawString(s, false))
        {
            return false;
        }
    }

    if (s->nextstate.token == token)
    {
        s->neednext = true;
        ExpandState(s);
        return true;
    }
    s->neednext = false;
    return false;
}

boolean SCN_TokensLeft(scanner_t *s)
{
    return s->scanpos < s->length;
}

const char *SCN_GetString(scanner_t *s)
{
    return s->state.string;
}

int SCN_GetNumber(scanner_t *s)
{
    return s->state.number;
}

boolean SCN_GetBoolean(scanner_t *s)
{
    return (boolean) s->state.number;
}

double SCN_GetDecimal(scanner_t *s)
{
    return s->state.decimal;
}

scanner_t *SCN_Open(const char *scriptname, const char *data, int length)
{
    scanner_t *s = calloc(1, sizeof(*s));

    s->line = 1;
    s->neednext = true;

    s->length = length;
    s->data = data;

    CheckForWhitespace(s);
    s->state.scanpos = s->scanpos;
    s->scriptname = M_StringDuplicate(scriptname);
    return s;
}

void SCN_Close(scanner_t *s)
{
    if (s->state.string)
    {
        free(s->state.string);
    }
    if (s->prevstate.string)
    {
        free(s->prevstate.string);
    }
    if (s->nextstate.string)
    {
        free(s->nextstate.string);
    }
    if (s->scriptname)
    {
        free((char *) s->scriptname);
    }
    free(s);
}

int SCN_GetNegativeInteger(scanner_t *s)
{
    boolean neg = SCN_CheckToken(s, '-');
    SCN_MustGetToken(s, TK_IntConst);
    return neg ? -SCN_GetNumber(s) : SCN_GetNumber(s);
}

double SCN_GetNegativeDecimal(scanner_t *s)
{
    boolean neg = SCN_CheckToken(s, '-');
    SCN_MustGetToken(s, TK_FloatConst);
    return neg ? -SCN_GetDecimal(s) : SCN_GetDecimal(s);
}
