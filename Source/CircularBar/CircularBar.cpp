#include <Rocket/CircularBar.h>
#include <Rocket/CircularBar/ElementCircularBar.h>
#include <Rocket/Core/ElementInstancerGeneric.h>
#include <Rocket/Core/Factory.h>
#include <Rocket/Core/StyleSheetSpecification.h>
#include <Rocket/Core/XMLParser.h>
#include <Rocket/Core/Plugin.h>
#include <Rocket/Core/Core.h>

namespace Rocket {
namespace CircularBar {

void RegisterElementInstancers()
{
	Core::ElementInstancer* instancer = new Core::ElementInstancerGeneric< ElementCircularBar >();
	Core::Factory::RegisterElementInstancer("circularbar", instancer);
	instancer->RemoveReference();
}

static bool initialised = false;

class CircularBarPlugin : public Rocket::Core::Plugin
{
public:
	void OnShutdown()
	{
		initialised = false;
		delete this;
	}

	int GetEventClasses()
	{
		return Rocket::Core::Plugin::EVT_BASIC;
	}
};

void Initialise()
{
	// Prevent double initialization
	if (!initialised)
	{
		RegisterElementInstancers();

		// Register the circular bar plug in, so we'll be notified on Shutdown
		Rocket::Core::RegisterPlugin(new CircularBarPlugin());

		initialised = true;
	}
}

}
}