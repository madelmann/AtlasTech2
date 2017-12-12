
#ifndef Scene_ConsolePlugin_h
#define Scene_ConsolePlugin_h


// Library includes

// Project includes
#include <Interfaces/Console/IConsolePlugin.h>
#include <Interfaces/Console/IConsolePrinter.h>
#include <Interfaces/Mesh/IObjectManager.h>
#include <Interfaces/Terrain/IManager.h>
#include <Math/Vector3.h>
#include <Parsers/Evaluator.h>
#include <Parsers/Valueizer.h>
#include <Renderer/RenderObject.h>
#include <Scene/Manager.h>
#include <Scene/SceneGraphObject.h>
#include <Scene/Settings.h>
#include <Tools/Files.h>

// Forward declarations

// Namespace declarations


namespace Scene {


class ConsolePlugin : public Console::IConsolePlugin
{
public:
	ConsolePlugin()
	: Console::IConsolePlugin("scene"),
	  mObjectManager(0),
	  mSceneManager(0),
	  mSettings(0),
	  mTerrainManager(0)
	{ }

public:
	void connectObjectManager(Mesh::IObjectManager *m) {
		mObjectManager = m;
	}
	void connectSceneManager(Manager *m) {
		mSceneManager = m;
		mSettings = mSceneManager->provideSettings();
	}
	void connectTerrainManager(Terrain::IManager *m) {
		mTerrainManager = m;
	}

	void execute(Valueizer *t) {
		assert(mPrinter);
		assert(mObjectManager);
		assert(mSceneManager);
		assert(mTerrainManager);

		if ( t->hasNext() ) {
			Value value = t->getTokenAsValue();

			if ( t->hasNext() ) {
				if ( value.ofValue("add") ) {
					std::string obj = t->getToken();

					if ( Tools::Files::ExtractFileExt(obj) != "ao" ) {
						obj += ".ao";
					}

					Core::RenderObject *object = mObjectManager->create(obj);
					if ( !object ) {
						mPrinter->print("could not load '" + obj + "'!");
						return;
					}

					SceneGraphObject *sgo = mSceneManager->create(object);
					if ( !sgo ) {
						mPrinter->print("could not add '" + obj + "' to scene!");
						return;
					}

					vector3f pos;
					if ( t->hasNext() ) {
						Value val;

						val = Value(t->getToken());
						pos.x = val.toFloat();
						if ( t->hasNext() ) {
							val = Value(t->getToken());
							pos.z = val.toFloat();
						}
					}
					
					sgo->init(mTerrainManager->heightAt(pos), vector3f());

					mSceneManager->attach(sgo, Core::invalidID, false);
				}
				if ( value.ofValue("addto") ) {
					int addToId = Value(t->getToken()).toInt();
					if ( addToId == Core::invalidID ) {
						mPrinter->print("invalid 'addTo' id provided!");
						return;
					}

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

						SceneGraphObject *sgo = mSceneManager->create(object);
						if ( !sgo ) {
							mPrinter->print("could not add '" + obj + "' to scene!");
							return;
						}

						vector3f pos;
						if ( t->hasNext() ) {
							Value val;

							val = Value(t->getToken());
							pos.x = val.toFloat();
							if ( t->hasNext() ) {
								val = Value(t->getToken());
								pos.z = val.toFloat();
							}
						}

						sgo->init(mTerrainManager->heightAt(pos), vector3f());

						mSceneManager->attach(sgo, addToId, false);
					}
				}
				else if ( value.ofValue("daytime") ) {
					value = Value(t->getToken());

					mSettings->setDayTime(value.toInt());

					mPrinter->print("Scene: daytime set to " + value.toString() + ".");
				}
				else if ( value.ofValue("editor") ) {
					value = Value(t->getToken());

					mSettings->setEditorMode(value.toBool());

					mPrinter->print("Scene: editor set to " + value.toString() + ".");
				}
				else if ( value.ofValue("fadeafter") ) {
					value = Value(t->getToken());

					mSettings->distanceFade(value.toFloat());

					mPrinter->print("Scene: fadeafter set to " + value.toString() + ".");
				}
				else if ( value.ofValue("hideafter") ) {
					value = Value(t->getToken());

					mSettings->distanceHide(value.toFloat());

					mPrinter->print("Scene: hideafter set to " + value.toString() + ".");
				}
			}
			else {
				if ( value.ofValue("daytime") ) {
					mPrinter->print(Tools::toString(mSettings->getDayTime()));
				}
				else if ( value.ofValue("editor") ) {
					mPrinter->print(Tools::toString(mSettings->getEditorMode()));
				}
				else if ( value.ofValue("fadeafter") ) {
					mPrinter->print(Tools::toString(mSettings->distanceFade()));
				}
				else if ( value.ofValue("hideafter") ) {
					mPrinter->print(Tools::toString(mSettings->distanceHide()));
				}
			}

			// return after all parameters have been processed
			return;
		}

		help();
	}

protected:

private:
	void help() {
		mPrinter->print("Usage: scene <setting> <value>");
		mPrinter->print("");
		mPrinter->print("Available settings:");
		mPrinter->print("   add <object>[.ao] [<vector3> = x z]");
		mPrinter->print("   addto <object>[.ao] [<vector3> = x z]");
		mPrinter->print("   daytime <int>");
		mPrinter->print("   editor <bool>");
		mPrinter->print("   fadeafter <float>");
		mPrinter->print("   hideafter <float>");
		mPrinter->print("");
	}

private:
	Mesh::IObjectManager	*mObjectManager;
	Manager					*mSceneManager;
	Settings				*mSettings;
	Terrain::IManager		*mTerrainManager;
};


}


#endif
