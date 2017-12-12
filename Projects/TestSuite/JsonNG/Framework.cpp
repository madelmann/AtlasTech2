
// Header
#include "Framework.h"

// Library includes

// Project includes
#include <JsonNG/Json.h>

// Namespace declartations


namespace Testing {
namespace JsonNG {


Framework::Framework(const Common::Logger *p)
: Common::Logger(p, "Framework")
{
}

void Framework::process()
{
	TEST(StorageTest);
	TEST(TransactionTest);
	TEST(ValueTest);
}

void Framework::setup()
{
}

void Framework::teardown()
{
}

void Framework::StorageTest()
{
	Json::Storage storage(this);

	Json::Value value;
	storage.load("game.json", value);

	value.addMember("id", 1);
	value.addMember("reason", "blabla");
	value.addMember("type", "object");

	storage.store(value);
}

void Framework::TransactionTest()
{
	Json::Connection connection(this);
	connection.open("fireandsmoke.json", "", "");
	Json::Value value = connection.read();

	std::string str = "{ \"id\": 1 }";
	value = Json::Parser::parse(str);

	try {
		connection.begin();
		connection.write(value);
		connection.commit();
	}
	catch  ( Json::Exceptions::JSONException &e ) {
		error(e.what());

		connection.rollback();
	}

	connection.close();
}

void Framework::ValueTest()
{
	Json::Value value;

	value.addMember("id", 1);
	value.addMember("method", "test");
	value.addMember("params", Json::Value());
	value["params"].addMember("array_null", Json::Value());
	value["params"]["array_string"][0] = "element1";
	value["params"]["array_string"][1] = "element2";
	value["params"]["array_string"][2] = "element3";
	value["params"]["array_int"][0] = 1;
	value["params"]["array_int"][1] = 2;
	value["params"]["array_int"][2] = 3;
	//value["params"]["array_object"][0].addMember("object", Json::Value());

	std::string v = value.toString();
	debug(v);

	Json::Value tmp = Json::Parser::parse(v);
	debug(tmp.toString());
}


}
}
