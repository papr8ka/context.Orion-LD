
// Generated from Query.g4 by ANTLR 4.9.2

#pragma once


#include "antlr4-runtime.h"




class  QueryParser : public antlr4::Parser {
public:
  enum {
    T__0 = 1, T__1 = 2, T__2 = 3, T__3 = 4, NUMBER = 5, FRAC = 6, QINT = 7, 
    ANYDIGIT = 8, DOTS = 9, DOT = 10, FALSE = 11, TRUE = 12, EQUAL = 13, 
    UNEQUAL = 14, GREATEREQ = 15, GREATER = 16, LESSEQ = 17, LESS = 18, 
    EXP = 19, DOTTEDATTRNAME = 20, ATTRNAME = 21, QUOTE = 22, ZERO = 23, 
    NONZERODIGIT = 24, PLUS = 25, MINUS = 26, OROP = 27, ANDOP = 28, DATE = 29, 
    TIME = 30, DATETIME = 31, URI = 32, SCHEME = 33, URICHAR = 34, PATTERNOP = 35, 
    PATTERNNOOP = 36, OPENINGPAR = 37, CLOSINGPAR = 38, ESCAPEDBACKSLASH = 39, 
    BACKSLASH = 40, OTHERLEXEDCHAR = 41, UNICODE = 42
  };

  enum {
    RuleQuery = 0, RuleQuerytermortermandassoc = 1, RuleQuerytermassocornot = 2, 
    RuleQuerytermassoc = 3, RuleQuerytermandassoc = 4, RuleQueryterm = 5, 
    RuleDottedpath = 6, RuleAttribute = 7, RuleQoperator = 8, RulePatternop = 9, 
    RulePatternnoop = 10, RuleGreatereq = 11, RuleGreater = 12, RuleLess = 13, 
    RuleLesseq = 14, RuleEqual = 15, RuleUnequal = 16, RuleOthervalue = 17, 
    RuleQtrue = 18, RuleQfalse = 19, RuleValue = 20, RuleRange = 21, RuleValuelist = 22, 
    RuleCompequalityvalue = 23, RuleUri = 24, RuleLogicalop = 25, RuleOrop = 26, 
    RuleAndop = 27, RuleComparablevalue = 28, RuleQuotedstr = 29, RuleNotaquote = 30, 
    RuleQuotedstringchar = 31, RuleRegex = 32, RuleRegexchar = 33, RuleNotaparenthesis = 34, 
    RuleDatetime = 35, RuleDate = 36, RuleTime = 37, RuleNumber = 38, RuleOpeningpar = 39, 
    RuleClosingpar = 40
  };

  explicit QueryParser(antlr4::TokenStream *input);
  ~QueryParser();

  virtual std::string getGrammarFileName() const override;
  virtual const antlr4::atn::ATN& getATN() const override { return _atn; };
  virtual const std::vector<std::string>& getTokenNames() const override { return _tokenNames; }; // deprecated: use vocabulary instead.
  virtual const std::vector<std::string>& getRuleNames() const override;
  virtual antlr4::dfa::Vocabulary& getVocabulary() const override;


  class QueryContext;
  class QuerytermortermandassocContext;
  class QuerytermassocornotContext;
  class QuerytermassocContext;
  class QuerytermandassocContext;
  class QuerytermContext;
  class DottedpathContext;
  class AttributeContext;
  class QoperatorContext;
  class PatternopContext;
  class PatternnoopContext;
  class GreatereqContext;
  class GreaterContext;
  class LessContext;
  class LesseqContext;
  class EqualContext;
  class UnequalContext;
  class OthervalueContext;
  class QtrueContext;
  class QfalseContext;
  class ValueContext;
  class RangeContext;
  class ValuelistContext;
  class CompequalityvalueContext;
  class UriContext;
  class LogicalopContext;
  class OropContext;
  class AndopContext;
  class ComparablevalueContext;
  class QuotedstrContext;
  class NotaquoteContext;
  class QuotedstringcharContext;
  class RegexContext;
  class RegexcharContext;
  class NotaparenthesisContext;
  class DatetimeContext;
  class DateContext;
  class TimeContext;
  class NumberContext;
  class OpeningparContext;
  class ClosingparContext; 

  class  QueryContext : public antlr4::ParserRuleContext {
  public:
    QueryContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    std::vector<QuerytermortermandassocContext *> querytermortermandassoc();
    QuerytermortermandassocContext* querytermortermandassoc(size_t i);
    antlr4::tree::TerminalNode *EOF();
    std::vector<OropContext *> orop();
    OropContext* orop(size_t i);

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  QueryContext* query();

  class  QuerytermortermandassocContext : public antlr4::ParserRuleContext {
  public:
    QuerytermortermandassocContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    std::vector<QuerytermassocornotContext *> querytermassocornot();
    QuerytermassocornotContext* querytermassocornot(size_t i);
    std::vector<AndopContext *> andop();
    AndopContext* andop(size_t i);

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  QuerytermortermandassocContext* querytermortermandassoc();

  class  QuerytermassocornotContext : public antlr4::ParserRuleContext {
  public:
    QuerytermassocornotContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    QuerytermassocContext *querytermassoc();
    QuerytermContext *queryterm();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  QuerytermassocornotContext* querytermassocornot();

  class  QuerytermassocContext : public antlr4::ParserRuleContext {
  public:
    QuerytermassocContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    OpeningparContext *openingpar();
    std::vector<QuerytermandassocContext *> querytermandassoc();
    QuerytermandassocContext* querytermandassoc(size_t i);
    ClosingparContext *closingpar();
    std::vector<OropContext *> orop();
    OropContext* orop(size_t i);

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  QuerytermassocContext* querytermassoc();

  class  QuerytermandassocContext : public antlr4::ParserRuleContext {
  public:
    QuerytermandassocContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    std::vector<QuerytermContext *> queryterm();
    QuerytermContext* queryterm(size_t i);
    std::vector<AndopContext *> andop();
    AndopContext* andop(size_t i);

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  QuerytermandassocContext* querytermandassoc();

  class  QuerytermContext : public antlr4::ParserRuleContext {
  public:
    QuerytermContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    AttributeContext *attribute();
    QoperatorContext *qoperator();
    ComparablevalueContext *comparablevalue();
    EqualContext *equal();
    CompequalityvalueContext *compequalityvalue();
    UnequalContext *unequal();
    PatternopContext *patternop();
    RegexContext *regex();
    PatternnoopContext *patternnoop();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  QuerytermContext* queryterm();

  class  DottedpathContext : public antlr4::ParserRuleContext {
  public:
    DottedpathContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *DOTTEDATTRNAME();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  DottedpathContext* dottedpath();

  class  AttributeContext : public antlr4::ParserRuleContext {
  public:
    AttributeContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    std::vector<DottedpathContext *> dottedpath();
    DottedpathContext* dottedpath(size_t i);

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  AttributeContext* attribute();

  class  QoperatorContext : public antlr4::ParserRuleContext {
  public:
    QoperatorContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    EqualContext *equal();
    UnequalContext *unequal();
    GreatereqContext *greatereq();
    GreaterContext *greater();
    LessContext *less();
    LesseqContext *lesseq();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  QoperatorContext* qoperator();

  class  PatternopContext : public antlr4::ParserRuleContext {
  public:
    PatternopContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *PATTERNOP();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  PatternopContext* patternop();

  class  PatternnoopContext : public antlr4::ParserRuleContext {
  public:
    PatternnoopContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *PATTERNNOOP();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  PatternnoopContext* patternnoop();

  class  GreatereqContext : public antlr4::ParserRuleContext {
  public:
    GreatereqContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *GREATEREQ();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  GreatereqContext* greatereq();

  class  GreaterContext : public antlr4::ParserRuleContext {
  public:
    GreaterContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *GREATER();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  GreaterContext* greater();

  class  LessContext : public antlr4::ParserRuleContext {
  public:
    LessContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *LESS();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  LessContext* less();

  class  LesseqContext : public antlr4::ParserRuleContext {
  public:
    LesseqContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *LESSEQ();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  LesseqContext* lesseq();

  class  EqualContext : public antlr4::ParserRuleContext {
  public:
    EqualContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *EQUAL();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  EqualContext* equal();

  class  UnequalContext : public antlr4::ParserRuleContext {
  public:
    UnequalContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *UNEQUAL();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  UnequalContext* unequal();

  class  OthervalueContext : public antlr4::ParserRuleContext {
  public:
    OthervalueContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    QfalseContext *qfalse();
    QtrueContext *qtrue();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  OthervalueContext* othervalue();

  class  QtrueContext : public antlr4::ParserRuleContext {
  public:
    QtrueContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *TRUE();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  QtrueContext* qtrue();

  class  QfalseContext : public antlr4::ParserRuleContext {
  public:
    QfalseContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *FALSE();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  QfalseContext* qfalse();

  class  ValueContext : public antlr4::ParserRuleContext {
  public:
    ValueContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    ComparablevalueContext *comparablevalue();
    OthervalueContext *othervalue();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  ValueContext* value();

  class  RangeContext : public antlr4::ParserRuleContext {
  public:
    RangeContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    std::vector<ComparablevalueContext *> comparablevalue();
    ComparablevalueContext* comparablevalue(size_t i);
    antlr4::tree::TerminalNode *DOTS();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  RangeContext* range();

  class  ValuelistContext : public antlr4::ParserRuleContext {
  public:
    ValuelistContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    std::vector<ValueContext *> value();
    ValueContext* value(size_t i);

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  ValuelistContext* valuelist();

  class  CompequalityvalueContext : public antlr4::ParserRuleContext {
  public:
    CompequalityvalueContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    OthervalueContext *othervalue();
    ValuelistContext *valuelist();
    RangeContext *range();
    UriContext *uri();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  CompequalityvalueContext* compequalityvalue();

  class  UriContext : public antlr4::ParserRuleContext {
  public:
    UriContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *URI();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  UriContext* uri();

  class  LogicalopContext : public antlr4::ParserRuleContext {
  public:
    LogicalopContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    OropContext *orop();
    AndopContext *andop();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  LogicalopContext* logicalop();

  class  OropContext : public antlr4::ParserRuleContext {
  public:
    OropContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *OROP();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  OropContext* orop();

  class  AndopContext : public antlr4::ParserRuleContext {
  public:
    AndopContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *ANDOP();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  AndopContext* andop();

  class  ComparablevalueContext : public antlr4::ParserRuleContext {
  public:
    ComparablevalueContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    NumberContext *number();
    QuotedstrContext *quotedstr();
    DatetimeContext *datetime();
    DateContext *date();
    TimeContext *time();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  ComparablevalueContext* comparablevalue();

  class  QuotedstrContext : public antlr4::ParserRuleContext {
  public:
    QuotedstrContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    std::vector<antlr4::tree::TerminalNode *> QUOTE();
    antlr4::tree::TerminalNode* QUOTE(size_t i);
    std::vector<QuotedstringcharContext *> quotedstringchar();
    QuotedstringcharContext* quotedstringchar(size_t i);

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  QuotedstrContext* quotedstr();

  class  NotaquoteContext : public antlr4::ParserRuleContext {
  public:
    NotaquoteContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *QUOTE();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  NotaquoteContext* notaquote();

  class  QuotedstringcharContext : public antlr4::ParserRuleContext {
  public:
    QuotedstringcharContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    NotaquoteContext *notaquote();
    antlr4::tree::TerminalNode *ESCAPEDBACKSLASH();
    antlr4::tree::TerminalNode *BACKSLASH();
    antlr4::tree::TerminalNode *QUOTE();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  QuotedstringcharContext* quotedstringchar();

  class  RegexContext : public antlr4::ParserRuleContext {
  public:
    RegexContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *CLOSINGPAR();
    std::vector<RegexcharContext *> regexchar();
    RegexcharContext* regexchar(size_t i);

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  RegexContext* regex();

  class  RegexcharContext : public antlr4::ParserRuleContext {
  public:
    RegexcharContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    NotaparenthesisContext *notaparenthesis();
    antlr4::tree::TerminalNode *BACKSLASH();
    antlr4::tree::TerminalNode *OPENINGPAR();
    antlr4::tree::TerminalNode *CLOSINGPAR();
    antlr4::tree::TerminalNode *ESCAPEDBACKSLASH();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  RegexcharContext* regexchar();

  class  NotaparenthesisContext : public antlr4::ParserRuleContext {
  public:
    NotaparenthesisContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *OPENINGPAR();
    antlr4::tree::TerminalNode *CLOSINGPAR();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  NotaparenthesisContext* notaparenthesis();

  class  DatetimeContext : public antlr4::ParserRuleContext {
  public:
    DatetimeContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *DATETIME();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  DatetimeContext* datetime();

  class  DateContext : public antlr4::ParserRuleContext {
  public:
    DateContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *DATE();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  DateContext* date();

  class  TimeContext : public antlr4::ParserRuleContext {
  public:
    TimeContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *TIME();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  TimeContext* time();

  class  NumberContext : public antlr4::ParserRuleContext {
  public:
    NumberContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *NUMBER();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  NumberContext* number();

  class  OpeningparContext : public antlr4::ParserRuleContext {
  public:
    OpeningparContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *OPENINGPAR();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  OpeningparContext* openingpar();

  class  ClosingparContext : public antlr4::ParserRuleContext {
  public:
    ClosingparContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *CLOSINGPAR();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  ClosingparContext* closingpar();


private:
  static std::vector<antlr4::dfa::DFA> _decisionToDFA;
  static antlr4::atn::PredictionContextCache _sharedContextCache;
  static std::vector<std::string> _ruleNames;
  static std::vector<std::string> _tokenNames;

  static std::vector<std::string> _literalNames;
  static std::vector<std::string> _symbolicNames;
  static antlr4::dfa::Vocabulary _vocabulary;
  static antlr4::atn::ATN _atn;
  static std::vector<uint16_t> _serializedATN;


  struct Initializer {
    Initializer();
  };
  static Initializer _init;
};

