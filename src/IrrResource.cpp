#include "IrrResource.h"

#include <irrlicht/ITexture.h>

TexRes::TexRes(): Resource() { }
TexRes::TexRes(String filename, String nameID, size_t sessionID):
		Resource(ResType::TEX, filename, nameID, sessionID) { }

void TexRes::load(irr::IrrlichtDevice* device) {
	if (device) {
		tex = device->getVideoDriver()->getTexture(getFilename().c_str());
		if (tex) {
			sheetTileDim = Coord::fromIrrDim(tex->getSize());
		}
	}
}
irr::video::ITexture* TexRes::getTexture() {
	return tex;
}

void TexRes::setSheetTileDim(Coord dim) {
	sheetTileDim = dim;
}
Coord TexRes::getSheetTileDim() const {
	return sheetTileDim;
}