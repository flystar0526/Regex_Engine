#include <cassert>

#include "regex.h"

int main() {
  Regex re1 = Regex("a(a|b)*c(d|e)(x|y|z)*");

  assert(re1.Match("aabbace"));
  assert(re1.Match("aabbbbbbace"));
  assert(re1.Match("aabbbbbbacd"));
  assert(!re1.Match("aabbbbbbad"));
  assert(!re1.Match("aabbbbbbaf"));
  assert(re1.Match("aabbbbbbacdxxx"));
  assert(re1.Match("aabbbbbbacdxzy"));
  assert(!re1.Match("aabbbbbbacdxzy1"));

  Regex re2 = Regex("aa*(b|c)*abc");

  assert(re2.Match("aababc"));
  assert(re2.Match("acabc"));
  assert(re2.Match("aabc"));
  assert(!re2.Match("aabaabc"));
  assert(re2.Match("abbabc"));
  assert(!re2.Match("aacx"));

  Regex re3 = Regex("abab(ab)*cd(e|f)*abc");

  assert(re3.Match("abababcdeabc"));
  assert(re3.Match("abababcdfabc"));
  assert(re3.Match("ababababababcdfabc"));
  assert(!re3.Match("abababababacdfabc"));
  assert(!re3.Match("abababababacdeabc"));

  Regex re4 = Regex("a+b*(c|d)?x+");

  assert(re4.Match("abcx"));
  assert(re4.Match("abx"));
  assert(!re4.Match("ab"));
  assert(re4.Match("aaabbdxxx"));

  Regex re5 = Regex("[a-zA-Z][0-9a-zA-Z]*");

  assert(re5.Match("abcd12345"));
  assert(re5.Match("name123"));
  assert(re5.Match("name1abac23"));
  assert(!re5.Match("0"));
  assert(!re5.Match("0abcd123"));
  assert(!re5.Match("02384"));
  assert(!re5.Match("a02384!"));

  Regex re6 = Regex("[a-z]([0-9]+|[A-Z])?");

  assert(re6.Match("b"));
  assert(re6.Match("a78"));
  assert(re6.Match("aZ"));
  assert(!re6.Match("aa"));
  assert(!re6.Match("a09a"));

  Regex re7 = Regex("((\\+|\\-)[0-9])?[0-9]*(([0-9].)|(.[0-9]))?[0-9]*");

  assert(re7.Match("+31.25"));
  assert(re7.Match("-31.25"));
  assert(!re7.Match("-.25"));
  assert(!re7.Match("-"));
  assert(re7.Match(".123"));
  assert(re7.Match("122."));
  assert(!re7.Match("1.1.1"));

  return 0;
}
