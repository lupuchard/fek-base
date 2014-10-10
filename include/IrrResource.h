#ifndef IRRRESOURCE_H
#define IRRRESOURCE_H

#include "Resource.h"

irr { video { class ITexture; } }

/**
 * An Irrlicht texture resource.
 */
class TexRes: public Resource {
public:
	TexRes();
	TexRes(String filename, String nameID, size_t sessionID);
	void load(irr::IrrlichtDevice* device = nullptr) override;
	irr::video::ITexture* getTexture();

	/**
	 * Sets the size of a single tile on this tilesheet texture.
	 * By default, this is set to the full size of the texture,
	 * which would mean it's just a picture and not a tilesheet.
	 */
	void setSheetTileDim(Coord dim);
	Coord getSheetTileDim() const;

private:
	irr::video::ITexture* tex = nullptr;
	Coord sheetTileDim;
};

#endif // IRRRESOURCE_H