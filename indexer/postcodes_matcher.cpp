#include "indexer/postcodes_matcher.hpp"
#include "indexer/search_delimiters.hpp"
#include "indexer/search_string_utils.hpp"
#include "indexer/string_set.hpp"

#include "base/logging.hpp"
#include "base/macros.hpp"
#include "base/stl_add.hpp"
#include "base/string_utils.hpp"

#include "std/transform_iterator.hpp"
#include "std/unique_ptr.hpp"
#include "std/utility.hpp"
#include "std/vector.hpp"

using namespace strings;

namespace search
{
namespace
{
// Top patterns for postcodes. See
// search/search_quality/clusterize_postcodes.lisp for details how
// these patterns were constructed.
char const * const g_patterns[] = {
    "aa nnnn",   "aa nnnnn",   "aaa nnnn",    "aan",      "aan naa",  "aana naa", "aann",
    "aann naa",  "aannaa",     "aannnaa",     "aannnn",   "an naa",   "ana naa",  "ana nan",
    "ananan",    "ann aann",   "ann naa",     "annnnaaa", "nn nnn",   "nnn",      "nnn nn",
    "nnn nnn",   "nnn nnnn",   "nnnn",        "nnnn aa",  "nnnn nnn", "nnnnaa",   "nnnnn",
    "nnnnn nnn", "nnnnn nnnn", "nnnnn nnnnn", "nnnnnn",   "nnnnnnn",  "nnnnnnnn", "〒nnn nnnn"};

UniChar SimplifyChar(UniChar const & c)
{
  if (IsASCIIDigit(c))
    return 'n';
  if (IsASCIILatin(c))
    return 'a';
  return c;
}

// This class puts all strings from g_patterns to a trie with a low
// branching factor and matches queries against these patterns.
class PostcodesMatcher
{
public:
  using TStringSet = StringSet<UniChar, 2>;

  PostcodesMatcher() : m_maxNumTokensInPostcode(0)
  {
    search::Delimiters delimiters;
    for (auto const * pattern : g_patterns)
      AddString(MakeUniString(pattern), delimiters);
  }

  bool HasString(StringSliceBase const & slice, bool isPrefix) const
  {
    auto status = m_strings.Has(make_transform_iterator(JoinIterator::Begin(slice), &SimplifyChar),
                                make_transform_iterator(JoinIterator::End(slice), &SimplifyChar));
    switch (status)
    {
    case TStringSet::Status::Absent: return false;
    case TStringSet::Status::Prefix: return isPrefix;
    case TStringSet::Status::Full: return true;
    }
  }

  inline size_t GetMaxNumTokensInPostcode() const { return m_maxNumTokensInPostcode; }

private:
  void AddString(UniString const & s, search::Delimiters & delimiters)
  {
    vector<UniString> tokens;
    SplitUniString(s, MakeBackInsertFunctor(tokens), delimiters);
    NoPrefixStringSlice slice(tokens);

    m_maxNumTokensInPostcode = max(m_maxNumTokensInPostcode, tokens.size());
    m_strings.Add(JoinIterator::Begin(slice), JoinIterator::End(slice));
  }

  TStringSet m_strings;
  size_t m_maxNumTokensInPostcode;

  DISALLOW_COPY(PostcodesMatcher);
};

PostcodesMatcher const & GetPostcodesMatcher()
{
  static PostcodesMatcher kMatcher;
  return kMatcher;
}
}  // namespace

bool LooksLikePostcode(StringSliceBase const & slice, bool isPrefix)
{
  return GetPostcodesMatcher().HasString(slice, isPrefix);
}

bool LooksLikePostcode(string const & s, bool isPrefix)
{
  vector<UniString> tokens;
  bool const lastTokenIsPrefix =
      TokenizeStringAndCheckIfLastTokenIsPrefix(s, tokens, search::Delimiters());

  return LooksLikePostcode(NoPrefixStringSlice(tokens), isPrefix && lastTokenIsPrefix);
}

size_t GetMaxNumTokensInPostcode() { return GetPostcodesMatcher().GetMaxNumTokensInPostcode(); }
}  // namespace search
