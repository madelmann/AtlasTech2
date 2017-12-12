
#ifndef Console_Plugins_SpawnObject_h
#define Console_Plugins_SpawnObject_h


// Library includes

// Project includes
#include <Interfaces/Console/IConsolePlugin.h>
#include <Interfaces/Console/IConsolePrinter.h>
#include <Interfaces/Mesh/IObjectManager.h>
#include <Interfaces/Scene/IManager.h>
#include <Interfaces/Terrain/IManager.h>
#include <Math/Vector3.h>
#include <Parsers/Value.h>
#include <Renderer/RenderObject.h>
#include <Scene/SceneGraphObject.h>
#include <Tools/Tools.h>

// Forward declarations

// Namespace declarations


class SpawnObject : public Console::IConsolePlugin
{
public:
	SpawnObject()
	: Console::IConsolePlugin("spawn"),
	  mObjectManager(0),
	  mSceneManager(0),
	  mTerrainManager(0)
	{ }

public:
	void connectObjectManager(Mesh::IObjectManager *m) {
		mObjectManager = m;
	}
	void connectSceneManager(Scene::IManager *m) {
		mSceneManager = m;
	}
	void connectTerrainManager(Terrain::IManager *m) {
		mTerrainManager = m;
	}

	void execute(Valueizer *t) {
		assert(mObjectManager);
		assert(mPrinter);
		assert(mSceneManager);
		assert(mTerrainManager);

		if ( t->hasNext() ) {
			std::string obj = t->getToken();

			if ( Tools::Files::ExtractFileExt(obj) != "ao" ) {
				obj += ".ao";
			}

			Core::RenderObject *object = mObjectManager->create(obj);
			if ( !object ) {
				mPrinter->print("could not load '" + obj + "'!");
				return;
			}

			Scene::SceneGraphObject *sgo = mSceneManager->create(object);
			if ( !sgo ) {
				mPrinter->print("could not add '" + obj + "' to scene!");
				return;
			}

			if ( t->hasNext() ) {
				vector3f pos;
				Value val;

				val = Value(t->getToken());
				pos.x = val.toFloat();
				if ( t->hasNext() ) {
					val = Value(t->getToken());
					pos.z = val.toFloat();

					if ( t->hasNext() ) {
						val = Value(t->getToken());
						pos.y = val.toFloat();
					}
					else {
						pos = mTerrainManager->heightAt(pos);
					}
				}

				sgo->init(pos, vector3f());
				mSceneManager->attach(sgo, Core::invalidID, false);
			}
			else {
				if ( t->getTokenAsValue().ofValue("help") ) {
					help();
				}
			}

			return;
		}

		mPrinter->print("invalid parameter size!");
		help();
	}

protected:

private:
	void help() {
		mPrinter->print("Usage: spawn <param1> [<param2> ...]");
		mPrinter->print("");
		mPrinter->print("Available commands:");
		mPrinter->print("   <object>[.ao] [<vector3> = x z y]");
	}

private:
	Mesh::IObjectManager	*mObjectManager;
	Scene::IManager			*mSceneManager;
	Terrain::IManager		*mTerrainManager;
};


#endif
