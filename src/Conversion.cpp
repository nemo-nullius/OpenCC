/*
 * Open Chinese Convert
 *
 * Copyright 2010-2014 Carbo Kuo <byvoid@byvoid.com>
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "Conversion.hpp"
#include "Dict.hpp"

using namespace opencc;

std::string Conversion::Convert(const char* phrase) const {
  std::ostringstream buffer;
  for (const char* pstr = phrase; *pstr != '\0';) {
    Optional<const DictEntry*> matched = dict->MatchPrefix(pstr);
    size_t matchedLength;
    if (matched.IsNull()) {
      matchedLength = UTF8Util::NextCharLength(pstr);
      buffer << UTF8Util::FromSubstr(pstr, matchedLength);
      buffer << 0;
    } else {
      matchedLength = matched.Get()->KeyLength();
      // buffer << matched.Get()->GetDefault();
      // append NumValues in the result the matched, including a phrase
      std::string DefaultResult = matched.Get()->GetDefault();
      for (const char* presult = DefaultResult.c_str(); *presult != '\0';) {
          size_t resultCharLength = UTF8Util::NextCharLength(presult);
          buffer << UTF8Util::FromSubstr(presult, resultCharLength);
          buffer << matched.Get()->NumValues();
          presult += resultCharLength;
      }
    }
    pstr += matchedLength;
  }
  return buffer.str();
}

std::string Conversion::Convert(const std::string& phrase) const {
  return Convert(phrase.c_str());
}

SegmentsPtr Conversion::Convert(const SegmentsPtr& input) const {
  SegmentsPtr output(new Segments);
  for (const char* segment : *input) {
    output->AddSegment(Convert(segment));
  }
  return output;
}
