#!/usr/bin/env python
# -*- encoding: utf-8 -*-
"""
Topic: 下降解析器
Desc :
"""
import re
import collections

# Token specification
REG = r'(?P<REG>e[abcdsi][xpi])'
NUM16 = r'(?P<NUM16>0x\w+)'
NUM = r'(?P<NUM>\d+)'
PLUS = r'(?P<PLUS>\+)'
MINUS = r'(?P<MINUS>-)'
TIMES = r'(?P<TIMES>\*)'
DIVIDE = r'(?P<DIVIDE>/)'
LPAREN = r'(?P<LPAREN>\()'
RPAREN = r'(?P<RPAREN>\))'
LSQUARE = r'(?P<LSQUARE>\[)'
RSQUARE = r'(?P<RSQUARE>\])'
WS = r'(?P<WS>\s+)'

master_pat = re.compile('|'.join([REG, NUM16, NUM, PLUS, MINUS, TIMES,
                                  DIVIDE, LPAREN, RPAREN, LSQUARE, RSQUARE, WS]))
# Tokenizer
Token = collections.namedtuple('Token', ['type', 'value'])


def generate_tokens(text):
    scanner = master_pat.scanner(text)
    for m in iter(scanner.match, None):
        tok = Token(m.lastgroup, m.group())
        if tok.type != 'WS':
            yield tok


# Parser
class ExpressionEvaluator(object):
    '''
    Implementation of a recursive descent parser. Each method
    implements a single grammar rule. Use the ._accept() method
    to test and accept the current lookahead token. Use the ._expect()
    method to exactly match and discard the next token on on the input
    (or raise a SyntaxError if it doesn't match).
    '''
    def __init__(self, handler=None):
        self.handler = handler

    def parse(self, text):
        self.tokens = generate_tokens(text)
        self.tok = None  # Last symbol consumed
        self.nexttok = None  # Next symbol tokenized
        self._advance()  # Load first lookahead token
        return self.expr()

    def _advance(self):
        'Advance one token ahead'
        self.tok, self.nexttok = self.nexttok, next(self.tokens, None)

    def _accept(self, toktype):
        'Test and consume the next token if it matches toktype'
        if self.nexttok and self.nexttok.type == toktype:
            self._advance()
            return True
        else:
            return False

    def _expect(self, toktype):
        'Consume next token if it matches toktype or raise SyntaxError'
        if not self._accept(toktype):
            raise SyntaxError('Expected ' + toktype)

    # Grammar rules follow
    def expr(self):
        "expression ::= term { ('+'|'-') term }*"
        exprval = self.term()
        while self._accept('PLUS') or self._accept('MINUS'):
            op = self.tok.type
            right = self.term()
            if op == 'PLUS':
                exprval += right
            elif op == 'MINUS':
                exprval -= right
        return exprval

    def term(self):
        "term ::= factor { ('*'|'/') factor }*"
        termval = self.factor()
        while self._accept('TIMES') or self._accept('DIVIDE'):
            op = self.tok.type
            right = self.factor()
            if op == 'TIMES':
                termval *= right
            elif op == 'DIVIDE':
                termval /= right
        return termval

    def factor(self):
        "factor ::= NUM | NUM16 | ( expr ) | REG"
        if self._accept('NUM'):
            return int(self.tok.value)
        elif self._accept('NUM16'):
            return int(self.tok.value, 16)
        elif self._accept('LPAREN'):
            exprval = self.expr()
            self._expect('RPAREN')
            return exprval
        elif self._accept('LSQUARE'):
            exprval = self.expr()
            self._expect('RSQUARE')
            if self.handler:
                return self.handler.handle_addr(exprval)
            return exprval
        elif self._accept('REG'):
            if self.handler:
                return self.handler.handle_reg(self.tok.value)
            return 0
        else:
            raise SyntaxError('Expected NUMBER or LPAREN')

class RecHandler(object):
    def __init__(self, imm, regs):
        self.regs = regs
        self.imm = imm

    def handle_reg(self, val):
        val_up = val.upper()
        if val_up in self.regs:
            return int(self.regs[val_up])

        return 0

    def handle_addr(self, val):
        if val == 0:
            return 0

        ret = self.imm.readLong(val)
        return ret


def parse_expr(imm, regs, expr):
    rh = RecHandler(imm, regs)
    ee = ExpressionEvaluator(rh)
    return ee.parse(expr)


def parse_exprs(imm, regs, exprs):
    exprs = exprs.split(',')
    rh = RecHandler(imm, regs)
    ee = ExpressionEvaluator(rh)
    rets = map(lambda x: '{}:{:x}'.format(x, ee.parse(x)), exprs)
    return ','.join(rets)


def descent_parser():
    e = ExpressionEvaluator()
    print(e.parse('[eax]'))
#     print(e.parse('2 + 3'))
#     print(e.parse('2 + 3 * 4'))
#     print(e.parse('2 + (3 + 4) * 5'))
    # print(e.parse('2 + (3 + * 4)'))
    # Traceback (most recent call last):
    #  File "<stdin>", line 1, in <module>
    #  File "exprparse.py", line 40, in parse
    #  return self.expr()
    #  File "exprparse.py", line 67, in expr
    #  right = self.term()
    #  File "exprparse.py", line 77, in term
    #  termval = self.factor()
    #  File "exprparse.py", line 93, in factor
    #  exprval = self.expr()
    #  File "exprparse.py", line 67, in expr
    #  right = self.term()
    #  File "exprparse.py", line 77, in term
    #  termval = self.factor()
    #  File "exprparse.py", line 97, in factor
    #  raise SyntaxError("Expected NUMBER or LPAREN")
    #  SyntaxError: Expected NUMBER or LPAREN


if __name__ == '__main__':
    descent_parser()
