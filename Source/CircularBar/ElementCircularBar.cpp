#include <Rocket/CircularBar/ElementCircularBar.h>
#include <Rocket/Core/GeometryUtilities.h>
#include <Rocket/Core/ElementDocument.h>
#include <Rocket/Core.h>

namespace Rocket {
namespace CircularBar {

// Constructs a new ElementCircularBar. This should not be called directly; use the Factory instead.
ElementCircularBar::ElementCircularBar(const Rocket::Core::String& tag) : Core::Element(tag), gauge_geometry(this), gauge_extent(-1, -1), geometry_dirty(true), value(0.0f)
{
}

ElementCircularBar::~ElementCircularBar()
{
}

// Returns float value between 0 and 1 of the circular bar.
float ElementCircularBar::GetValue() const
{
	return GetAttribute< float >("value", 0.0f);
}

// Sets the current value of the circular bar.
void ElementCircularBar::SetValue(const float value)
{
	SetAttribute("value", value);
}

// Called during render after backgrounds, borders, decorators, but before children, are rendered.
void ElementCircularBar::OnRender()
{
	if(geometry_dirty)
		GenerateGeometry();

	gauge_geometry.Render(GetAbsoluteOffset(Rocket::Core::Box::CONTENT));
}

// Called when attributes on the element are changed.
void ElementCircularBar::OnAttributeChange(const Core::AttributeNameList& changed_attributes)
{
	Element::OnAttributeChange(changed_attributes);

	if (changed_attributes.find("value") != changed_attributes.end())
	{
		value = GetAttribute< float >("value", 0.0f);

		if (value < 0)
			value = 0.0f;
		else if (value > 1)
			value = 1.0f;

		geometry_dirty = true;
	}
}

// Called when properties on the element are changed.
void ElementCircularBar::OnPropertyChange(const Core::PropertyNameList& changed_properties)
{
	Element::OnPropertyChange(changed_properties);

	if (changed_properties.find("gauge-src") != changed_properties.end())
	{
		LoadTexture();
	}

	if(changed_properties.find("width") != changed_properties.end())
	{
		int width = GetProperty< int >("width");

		gauge_extent.x = width;

		geometry_dirty = true;
	}

	if(changed_properties.find("height") != changed_properties.end())
	{
		int height = GetProperty< int >("height");

		gauge_extent.y = height;

		geometry_dirty = true;
	}
}

// Called when gauge has been filled.
void ElementCircularBar::GenerateGeometry()
{
	int current_vertex_count;
	float filled_angle;
	float progression;
	float tangent;
	Core::Vector2i half_size;

	Rocket::Core::Vertex current_vertex;

	gauge_geometry.Release(true);

	std::vector< Rocket::Core::Vertex >& vertices = gauge_geometry.GetVertices();
	std::vector< int >& indices = gauge_geometry.GetIndices();

	current_vertex_count = 0;

	progression = Core::Math::Clamp<float>(value, 0.0f, 1.0f);

	filled_angle = progression * Core::Math::TWO_PI;

	vertices.resize(10);

	half_size.x = gauge_extent.x != -1 ? gauge_extent.x : actual_gauge_extent.x;
	half_size.y = gauge_extent.y != -1 ? gauge_extent.y : actual_gauge_extent.y;
	half_size.x = (int)(half_size.x * 0.5f);
	half_size.y = (int)(half_size.y * 0.5f);

	current_vertex.colour = Core::Colourb(255, 255, 255, 255);
	current_vertex.tex_coord = Core::Vector2f(0.5f, 0.5f);
	current_vertex.position = Core::Vector2f((float)half_size.x, (float)half_size.y);

	vertices[0] = current_vertex;

	current_vertex.tex_coord = Core::Vector2f(0.5f, 0.0f);
	current_vertex.position = Core::Vector2f((float)half_size.x, 0.0f);

	vertices[1] = current_vertex;

	++current_vertex_count;

	if(filled_angle < Core::Math::DegreesToRadians(45.0f))
	{
		tangent = Core::Math::Tan(filled_angle);

		current_vertex.tex_coord = Core::Vector2f(0.5f + tangent * 0.5f, 0.0f);
		current_vertex.position = Core::Vector2f(half_size.x + half_size.x * tangent , 0.0f);

		vertices[2] = current_vertex;
	}
	else
	{
		current_vertex.tex_coord = Core::Vector2f(1.0f, 0.0f);
		current_vertex.position = Core::Vector2f((float)half_size.x + half_size.x, 0.0f );

		vertices[2] = current_vertex;

		++current_vertex_count;

		if (filled_angle < Core::Math::DegreesToRadians(90.0f))
		{
			tangent = Core::Math::Tan(Core::Math::DegreesToRadians(90.0f) - filled_angle);

			current_vertex.tex_coord = Core::Vector2f(1.0f, 0.5f - tangent * 0.5f);
			current_vertex.position = Core::Vector2f((float)half_size.x + half_size.x, half_size.y - half_size.y * tangent);

			vertices[3] = current_vertex;
		}
		else
		{
			current_vertex.tex_coord = Core::Vector2f(1.0f, 0.5f);
			current_vertex.position = Core::Vector2f((float)half_size.x + half_size.x, (float)half_size.y);

			vertices[3] = current_vertex;

			++current_vertex_count;

			if (filled_angle < Core::Math::DegreesToRadians(135.0f))
			{
				tangent = Core::Math::Tan(filled_angle - Core::Math::DegreesToRadians(90.0f) );

				current_vertex.tex_coord = Core::Vector2f(1.0f, 0.5f + tangent * 0.5f);
				current_vertex.position = Core::Vector2f((float)half_size.x + half_size.x, half_size.y + half_size.y * tangent);

				vertices[4] = current_vertex;
			}
			else
			{
				current_vertex.tex_coord = Core::Vector2f(1.0f, 1.0f);
				current_vertex.position = Core::Vector2f((float)half_size.x + half_size.x, (float)half_size.y + half_size.y);

				vertices[4] = current_vertex;

				++current_vertex_count;

				if (filled_angle < Core::Math::DegreesToRadians(180.0f))
				{
					tangent = Core::Math::Tan(Core::Math::DegreesToRadians(180.0f) - filled_angle );

					current_vertex.tex_coord = Core::Vector2f(0.5f + tangent * 0.5f, 1.0f);
					current_vertex.position = Core::Vector2f((float)half_size.x + half_size.x * tangent, (float)half_size.y + half_size.y);

					vertices[5] =current_vertex;
				}
				else
				{
					current_vertex.tex_coord = Core::Vector2f(0.5f, 1.0f);
					current_vertex.position = Core::Vector2f((float)half_size.x, (float)half_size.y + half_size.y);

					vertices[5] = current_vertex;

					++current_vertex_count;

					if ( filled_angle < Core::Math::DegreesToRadians(225.0f) )
					{
						tangent = Core::Math::Tan(filled_angle - Core::Math::DegreesToRadians(180.0f));

						current_vertex.tex_coord = Core::Vector2f(0.5f - tangent * 0.5f, 1.0f );
						current_vertex.position = Core::Vector2f(half_size.x - half_size.x * tangent, (float)half_size.y + half_size.y );

						vertices[6] = current_vertex;
					}
					else
					{
						current_vertex.tex_coord = Core::Vector2f(0.0f, 1.0f);
						current_vertex.position = Core::Vector2f(0.0f, (float)half_size.y + half_size.y);

						vertices[6] = current_vertex;

						++current_vertex_count;

						if ( filled_angle < Core::Math::DegreesToRadians(270.0f) )
						{
							tangent = Core::Math::Tan(Core::Math::DegreesToRadians(270.0f) - filled_angle);

							current_vertex.tex_coord = Core::Vector2f(0.0f, 0.5f + tangent * 0.5f );
							current_vertex.position = Core::Vector2f(0.0f, half_size.y + half_size.y * tangent);

							vertices[7] = current_vertex;
						}
						else
						{
							current_vertex.tex_coord = Core::Vector2f(0.0f, 0.5f);
							current_vertex.position = Core::Vector2f(0.0f, (float)half_size.y);

							vertices[7] = current_vertex;

							++current_vertex_count;

							if ( filled_angle < Core::Math::DegreesToRadians(315.0f) )
							{
								tangent = Core::Math::Tan(filled_angle - Core::Math::DegreesToRadians(270.0f));

								current_vertex.tex_coord = Core::Vector2f(0.0f, 0.5f - tangent * 0.5f);
								current_vertex.position = Core::Vector2f(0.0f, half_size.y - half_size.y * tangent);

								vertices[8] = current_vertex;
							}
							else
							{
								current_vertex.tex_coord = Core::Vector2f(0.0f, 0.0f);
								current_vertex.position = Core::Vector2f(0.0f, 0.0f);

								vertices[8] = current_vertex;

								++current_vertex_count;

								if ( filled_angle < Core::Math::DegreesToRadians(360.0f) )
								{
									tangent = Core::Math::Tan(Core::Math::DegreesToRadians(360.0f) - filled_angle);

									current_vertex.tex_coord = Core::Vector2f(0.5f - tangent * 0.5f, 0.0f);
									current_vertex.position = Core::Vector2f((float)half_size.x - half_size.x * tangent , 0.0f);

									vertices[9] = current_vertex;
								}
								else
								{
									current_vertex.tex_coord = vertices[ 1 ].tex_coord;
									current_vertex.position = vertices[ 1 ].position;

									vertices[9] = current_vertex;
								}
							}
						}
					}
				}
			}
		}
	}

	indices.resize(current_vertex_count * 3);
	for(int vertex_index = 0; vertex_index < current_vertex_count; vertex_index++)
	{
		indices[vertex_index * 3] = 0;
		indices[vertex_index * 3 + 1] = vertex_index + 1;
		indices[vertex_index * 3 + 2] = vertex_index + 2;
	}

	geometry_dirty = false;
}

/// Called when source texture has changed.
void ElementCircularBar::LoadTexture()
{
	Core::ElementDocument* document = GetOwnerDocument();
	Core::URL source_url(document == NULL ? "" : document->GetSourceURL());

	Core::String source = GetProperty< Core::String >("gauge-src");

	if (!gauge_texture.Load(source, source_url.GetPath()))
	{
		gauge_geometry.SetTexture(NULL);
		return;
	}

	gauge_geometry.SetTexture(&gauge_texture);
	actual_gauge_extent = gauge_texture.GetDimensions(GetRenderInterface());
}

}
}
