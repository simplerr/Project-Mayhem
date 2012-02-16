#pragma once

#include "enums.h"
#include "d3dUtil.h"

//! Abstract base class for all classes that are slot items.
//! Skills, Items inherits from this class.
class SlotItem
{
public:
	SlotItem();
	virtual ~SlotItem();

	virtual IDirect3DTexture9* getTexture() = 0;
	virtual SlotId	getSlotId() = 0;

	void	setId(int id);
	void	setSlotId(SlotId id);
	int		getId();
private:
	int					mId;
	SlotId				mSlotId;
};