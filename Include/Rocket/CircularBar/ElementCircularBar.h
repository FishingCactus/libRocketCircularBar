#ifndef ROCKETCIRCULARBARELEMENTCIRCULARBAR_H
#define ROCKETCIRCULARBARELEMENTCIRCULARBAR_H

#include <Rocket/CircularBar/Header.h>
#include <Rocket/Core/Element.h>
#include <Rocket/Core/Geometry.h>
#include <Rocket/Core/Texture.h>

namespace Rocket {
namespace CircularBar {

/**
	A circular progress bar.

	@author Jeanneteau Silouane
*/

class ROCKETCIRCULARBAR_API ElementCircularBar : public Core::Element
{
public:
	/// Constructs a new ElementCircularBar. This should not be called directly; use the Factory instead.
	/// @param[in] tag The tag the element was declared as in RML.
	ElementCircularBar(const Rocket::Core::String& tag);
	virtual ~ElementCircularBar();

	/// Returns float value between 0 and 1.
	/// @return The value of the circular bar.
	float GetValue() const;
	/// Sets the current value of the circular bar.
	/// @param[in] value The new value of the circular bar.
	void SetValue(const float value);

protected:
	/// Called during render after backgrounds, borders, decorators, but before children, are rendered.
	virtual void OnRender();

	/// Called when attributes on the element are changed.
	/// @param[in] changed_attributes The attributes changed on the element.
	virtual void OnAttributeChange(const Core::AttributeNameList& changed_attributes);
	/// Called when properties on the element are changed.
	/// @param[in] changed_properties The properties changed on the element.
	virtual void OnPropertyChange(const Core::PropertyNameList& changed_properties);

private:
	// Called when gauge has been filled.
	void GenerateGeometry();

	/// Called when source texture has changed.
	void LoadTexture();

	// Texture used for the gauge when a cursor hover the element
	Core::Texture gauge_texture;

	// The geometry used to render the gauge.
	Core::Geometry gauge_geometry;

	// The extent of the gauge set by the user.
	Core::Vector2i gauge_extent;

	// The actual extent of the gauge.
	Core::Vector2i actual_gauge_extent;

	// True if the geometry need to be generated
	bool geometry_dirty;

	// The progression of the bar between 0.0f and 1.0f.
	float value;
};

}
}

#endif
