#ifndef JSON_PARSER_H
#define JSON_PARSER_H

#include <fstream>
#include <exception>

#include <string.h>
#include <math.h>

#include "JSON.h"

namespace JSON {

	class Parser {
	public:
		static char nextIs(std::istream &stream, bool read = false) {
			char c = stream.peek();
			
			while (c == '\t' || c == '\n' || c == '\r' || c == ' ') {
				stream.ignore(1);
				c = stream.peek();
			}
			
			if (read) {
				return stream.get();
			}

			return c;
		}

		static String *parseString(std::istream &stream) {
			if (nextIs(stream, true) != '"') {
				throw std::exception();
			}

			String *str = new String("");
			while (stream.peek() != '"') {
				char c = stream.get();

				if (c == '\\') {
					c = stream.get();
					if (c == 'u') {
						stream.ignore(4);
						continue;
					} else if (c == 'b') {
						c = '\b';
					} else if (c == 'n') {
						c = '\n';
					} else if (c == 'r') {
						c = '\r';
					} else if (c == 't') {
						c = '\t';
					}
				}

				str->append(&c, 1);
			}
			stream.ignore(1);

			return str;
		}

		static Boolean *parseBoolean(std::istream &stream) {
			char buffer[5];
			
			if (nextIs(stream) == 't') {
				stream.read(buffer, 4);

				if (memcmp(buffer, "true", 4)) {
					return new Boolean(true);
				}
			} else if (nextIs(stream) == 'f') {
				stream.read(buffer, 5);

				if (memcmp(buffer, "false", 5)) {
					return new Boolean(false);
				}
			}
			
			throw std::exception();
		}

		static Value *parseNull(std::istream &stream) {
			char buffer[4];

			if (nextIs(stream) == 'n') {
				stream.read(buffer, 4);

				if (memcmp(buffer, "null", 4)) {
					return new Value();
				}
			}

			throw std::exception();
		}

		static Array *parseArray(std::istream &stream) {
			if (nextIs(stream, true) != '[') {
				throw std::exception();
			}

			Array *arr = new Array();

			while (nextIs(stream) != ']') {
				arr->push_back(parseValue(stream));

				if (nextIs(stream) == ',') {
					stream.ignore(1);
				}
			}
			stream.ignore(1);

			return arr;
		}

		static Number *parseNumber(std::istream &stream) {
			int sign = 1;
			int preDecimal = 0;
			int postDecimal = 0;
			int exponentSign = 1;
			int exponent = 0;
			int numTens = 0;

			bool hasPostDecimal = false;
			bool hasExponent = false;

			// Sign
			if (nextIs(stream) == '-') {
				sign = -1;
				stream.ignore(1);
			}

			// Pre-decimal
			if (nextIs(stream) == '0') {
				stream.ignore(1);

				if (nextIs(stream) >= '0' && nextIs(stream) <= '9') {
					throw std::exception();
				}
			} else if (nextIs(stream) > '0' && nextIs(stream) <= '9') {
				preDecimal = nextIs(stream, true) - '0';

				while (nextIs(stream) >= '0' && nextIs(stream) <= '9') {
					preDecimal = preDecimal * 10 + nextIs(stream, true) - '0';
				}
			}

			// Post-decimal
			if (nextIs(stream) == '.') {
				stream.ignore(1);
				hasPostDecimal = true;

				if (!(nextIs(stream) >= '0' && nextIs(stream) <= '9')) {
					throw std::exception();
				}

				while (nextIs(stream) >= '0' && nextIs(stream) <= '9') {
					postDecimal = postDecimal * 10 + nextIs(stream, true) - '0';
					numTens += 1;
				}
			}

			// Exponent
			if (nextIs(stream) == 'e' || nextIs(stream) == 'E') {
				stream.ignore(1);
				hasExponent = true;

				if (nextIs(stream) == '-') {
					exponentSign = -1;
					stream.ignore(1);
				} else if (nextIs(stream) == '+') {
					stream.ignore(1);
				}

				if (!(nextIs(stream) >= '0' && nextIs(stream) <= '9')) {
					throw std::exception();
				}

				while (nextIs(stream) >= '0' && nextIs(stream) <= '9') {
					exponent = exponent * 10 + nextIs(stream, true) - '0';
				}
			}

			if (hasExponent || hasPostDecimal) {
				float v = (float)(sign * preDecimal);
				
				if (hasPostDecimal) {
					float p = (float)postDecimal;

					while (p > 1.0) {
						p /= 10.0;
					}

					v += sign * p;
				}

				if (hasExponent) {
					v = v * pow(10.0f, (float)(exponentSign * exponent));
				}

				return new Number(v);
			} else {
				return new Number(preDecimal);
			}
		}

		static Value *parseValue(std::istream &stream) {
			char c = nextIs(stream);

			Value *value;

			if (c == '{') {
				value = parseObject(stream);
			} else if (c == '[') {
				value = parseArray(stream);
			} else if (c == '"') {
				value = parseString(stream);
			} else if (c == 't') {
				value = parseBoolean(stream);
			} else if (c == 'f') {
				value = parseBoolean(stream);
			} else if (c == 'n') {
				value = parseNull(stream);
			} else {
				value = parseNumber(stream);
			}

			return value;
		}

		static Object *parseObject(std::istream &stream) {
			if (nextIs(stream, true) != '{') {
				throw std::exception();
			}

			Object *obj = new Object();

			while (nextIs(stream) != '}') {
				if (nextIs(stream) == '"') {
					String *str = parseString(stream);
					
					if (nextIs(stream, true) != ':') {
						throw std::exception();
					}

					Value *value = parseValue(stream);

					(*obj)[*str] = value;
				} else if (nextIs(stream) == ',') {
					stream.ignore(1);
					continue;
				} else {
					throw std::exception();
				}
			}
			stream.ignore();

			return obj;
		}

	public:
		static Object *parse(const char *file) {
			std::ifstream stream(file, std::ios::binary);
			
			// File not found, or not opened
			if (!stream.is_open()) {
				throw std::exception();
			}

			Object *obj = parseObject(stream);

			return obj;
		}
	};

}

#endif