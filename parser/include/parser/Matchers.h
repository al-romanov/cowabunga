#ifndef PARSER_MATCHERS_H
#define PARSER_MATCHERS_H

#include "common/Token.h"
#include "parser/IMatcher.h"

#include <type_traits>

namespace cb {

namespace parser {

template <int TokenID>
class Terminal : public IMatcher {
public:
  TokenIterator match(TokenIterator ItBegin, TokenIterator ItEnd) override {
    if (ItBegin == ItEnd || ItBegin->id() != TokenID) {
      return ItBegin;
    }
    return ItBegin + 1;
  }
};

template <int RangeBeginID, int RangeEndID>
class TerminalInRange : public IMatcher {
public:
  TokenIterator match(TokenIterator ItBegin, TokenIterator ItEnd) override {
    if (ItBegin == ItEnd || ItBegin->id() <= RangeBeginID ||
        ItBegin->id() >= RangeEndID) {
      return ItBegin;
    }
    return ItBegin + 1;
  }
};

template <class TFirstMatcher, class... TOtherMatchers>
class Sequence : public IMatcher {
public:
  TokenIterator match(TokenIterator ItBegin, TokenIterator ItEnd) override {
    return matchSequence<TFirstMatcher, TOtherMatchers...>(ItBegin, ItEnd);
  }

private:
  template <class TLHSMatcher, class TRHSMatcher, class... TRemainingMatchers>
  TokenIterator matchSequence(TokenIterator ItBegin, TokenIterator ItEnd) {
    TLHSMatcher LHSMatcher;
    auto ItLHSMatchedEnd = LHSMatcher.match(ItBegin, ItEnd);
    if (ItLHSMatchedEnd == ItBegin) {
      return ItBegin;
    }
    auto ItOtherMatchedEnd =
        matchSequence<TRHSMatcher, TRemainingMatchers...>(ItLHSMatchedEnd, ItEnd);
    if (ItLHSMatchedEnd == ItOtherMatchedEnd) {
      return ItBegin;
    }
    return ItOtherMatchedEnd;
  }

  template <class TMatcher>
  TokenIterator matchSequence(TokenIterator ItBegin, TokenIterator ItEnd) {
    TMatcher Rule;
    return Rule.match(ItBegin, ItEnd);
  }
};

template <class TFirstMatcher, class TSecondMatcher, class... TOtherMatchers>
class Optional : public IMatcher {
public:
  TokenIterator match(TokenIterator ItBegin, TokenIterator ItEnd) override {
    if (ItBegin == ItEnd) {
      return ItBegin;
    }
    return matchOptions<TFirstMatcher, TSecondMatcher, TOtherMatchers...>(ItBegin, ItEnd);
  }

  template <class TCallback>
  void applyToExatcMatchedOption(TCallback &&Callback, TokenIterator ItBegin, TokenIterator ItEnd) {
    callbackOnExactMatch<TCallback, TFirstMatcher, TSecondMatcher, TOtherMatchers...>(Callback, ItBegin, ItEnd);
  }

private:
  template <class TLHSRule, class TRHSRule, class... TRemainingRules>
  TokenIterator matchOptions(TokenIterator ItBegin, TokenIterator ItEnd) {
    TLHSRule LHSRule;
    auto ItLHSMatchedEnd = LHSRule.match(ItBegin, ItEnd);
    auto ItRHSMatchedEnd =
        matchOptions<TRHSRule, TRemainingRules...>(ItBegin, ItEnd);
    if (ItLHSMatchedEnd - ItRHSMatchedEnd < 0) {
      return ItRHSMatchedEnd;
    }
    return ItLHSMatchedEnd;
  }

  template <class TRule>
  TokenIterator matchOptions(TokenIterator ItBegin, TokenIterator ItEnd) {
    TRule Rule;
    return Rule.match(ItBegin, ItEnd);
  }

  template <class TCallback, class TLHSRule, class TRHSRule, class... TRemainingRules>
  void callbackOnExactMatch(TCallback &&Callback, TokenIterator ItBegin, TokenIterator ItEnd) {
    TLHSRule LHSRule;
    auto ItLHSMatchedEnd = LHSRule.match(ItBegin, ItEnd);
    if (ItLHSMatchedEnd == ItEnd) {
      Callback(LHSRule);
    }
    callbackOnExactMatch<TCallback, TRHSRule, TRemainingRules...>(Callback, ItBegin, ItEnd);
  }

  template <class TCallback, class TRule>
  void callbackOnExactMatch(TCallback &&Callback, TokenIterator ItBegin, TokenIterator ItEnd) {
    TRule Rule;
    auto ItMatchedEnd = Rule.match(ItBegin, ItEnd);
    if (ItMatchedEnd == ItEnd) {
      Callback(Rule);
    }
  }
};

} // namespace parser

} // namespace cb

#endif
