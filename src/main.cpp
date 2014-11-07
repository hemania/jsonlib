#include "JSON.h"
#include "JSONParser.h"

#include <sstream>

int main() {
	JSON::Object *obj2 = JSON::Parser::parse("json.txt");

	JSON::String *name = obj2->at("name")->asString();
	JSON::Object *armor = obj2->at("armor")->asObject();

	printf("%s\n", name->c_str());
	printf("%d\n", armor->at("min")->asNumber()->asInt());

	while (true);
}