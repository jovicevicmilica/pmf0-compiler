#ifndef ERRORS_H
#define ERRORS_H

void reportError(const char* message, int line, int column);
void reportLexicalError(const char* message, int line, int column);

#endif
