#include "Collision.h"

MTV checkCollision (cPolygon poly1, cPolygon poly2) 
{
	MTV mtv;
	mtv.collision = false;

	cPolygon tmp1 = poly1;
	cPolygon tmp2 = poly2;

	tmp1.rotate(-tmp1.getRotation());
	tmp2.rotate(-tmp2.getRotation());

	Rect Rect1 = poly1.getBoundingBox();
	Rect Rect2 = poly2.getBoundingBox();
	Vector axis;			// Axis we will project onto
	Vector projection;	// The direction of the projection
	int i;			// Current i 
	double minA, maxA, minB, maxB, axisLen, tmp, minLen = 999999, tmpDepth = 1;

	if(Rect1.left > Rect2.right || Rect1.right < Rect2.left || Rect1.bottom < Rect2.top || Rect1.top > Rect2.bottom) {
		return mtv;
	}

	for( int i = 0; i < poly1.mVertices.size(); i++) 
	{
		if(i == 0) 
		{
			axis.x = -(poly1.mVertices[poly1.mVertices.size()-1].y - poly1.mVertices[0].y);
			axis.y = poly1.mVertices[poly1.mVertices.size()-1].x - poly1.mVertices[0].x;
		}
		else
		{
			axis.x = -(poly1.mVertices[i-1].y - poly1.mVertices[i].y);
			axis.y = poly1.mVertices[i-1].x - poly1.mVertices[i].x;
		}
		axisLen = sqrt(axis.x * axis.x + axis.y * axis.y);
		axis.x /= axisLen;
		axis.y /= axisLen;

		minA = maxA = (poly1.mVertices[0].x * axis.x + poly1.mVertices[0].y * axis.y);

		for(int i = 1; i < poly1.mVertices.size(); i++)
		{
			tmp = (poly1.mVertices[i].x * axis.x +poly1.mVertices[i].y * axis.y);

			if(tmp < minA)
				minA = tmp;
			else if (tmp > maxA)
				maxA = tmp;
		}

		/* correct with offset */
		tmp = poly1.getPos().x * axis.x + poly1.getPos().y * axis.y;
		minA += tmp;
		maxA += tmp;

		/* project Shape B's points on the axis to find MIN and MAX points */
		minB = maxB = (poly2.mVertices[0].x * axis.x + poly2.mVertices[0].y * axis.y);

		for(int i = 1; i < poly2.mVertices.size(); i++)
		{		
			tmp = poly2.mVertices[i].x * axis.x + poly2.mVertices[i].y * axis.y;		

			if(tmp < minB)
				minB = tmp;
			else if (tmp > maxB)
				maxB = tmp;
		}		

		/* correct with offset */
		tmp = poly2.getPos().x * axis.x + poly2.getPos().y * axis.y;
		minB += tmp;
		maxB += tmp;

		/* test if they intersect, if not then store the penetration depth and axis */
		if(minA > maxB || maxA < minB)	{
			mtv.collision = false;
			return mtv;
		}
		else
		{	
			// the projection axis is to be used by the player (objectB)
			if(maxA < maxB)	
			{
				tmpDepth = maxA - minB;
				if(tmpDepth < minLen)	{
					minLen = tmpDepth;
					projection = axis;
				}	
			}
			else if(maxA > maxB)
			{
				tmpDepth = maxB - minA;			
				if(tmpDepth < minLen)	{
					minLen = tmpDepth;
					projection.x = axis.x*-1;	// WHY? - to correct for ShapeB
					projection.y = axis.y*-1;					
				}
			}
		}
	}

	/* the same thing again but now for object B, sometimes a collision from Shape A isn't a collision for Shape B */
	/*
		- see above for detailed comments
	*/

	// test Shape B's sides
	for(i = 0; i < poly2.mVertices.size(); i++)
	{
		/* get the axis we will project on */
		if(i == 0)	// the first point must get connected to the last
		{			
			axis.x = poly2.mVertices[poly2.mVertices.size()-1].y - poly2.mVertices[0].y;
			axis.y = poly2.mVertices[0].x - poly2.mVertices[poly2.mVertices.size()-1].x;
		}
		else
		{
			if(i == 2)
				i = 2;

			axis.x = poly2.mVertices[i-1].y - poly2.mVertices[i].y;
			axis.y = poly2.mVertices[i].x - poly2.mVertices[i-1].x;	// should be reversed?:O
		}

		/* normalize the axis */	
		axisLen = sqrt(axis.x * axis.x + axis.y * axis.y);
		axis.x /= axisLen;
		axis.y /= axisLen;

		/* project Shape B's points on the axis to find MIN and MAX points */		
		minB = maxB = (poly2.mVertices[0].x * axis.x + poly2.mVertices[0].y * axis.y);

		for(int i = 1; i < poly2.mVertices.size(); i++)
		{
			tmp = (poly2.mVertices[i].x * axis.x + poly2.mVertices[i].y * axis.y);			

			if(tmp < minB)
				minB = tmp;
			else if (tmp > maxB)
				maxB = tmp;
		}

		/* correct with offset */
		tmp = poly2.getPos().x * axis.x + poly2.getPos().y * axis.y;
		minB += tmp;
		maxB += tmp;

		/* project Shape A's points on the axis to find MIN and MAX points */	
		minA = maxA = (poly1.mVertices[0].x * axis.x + poly1.mVertices[0].y * axis.y);

		for(int i = 1; i < poly1.mVertices.size(); i++)
		{	
			tmp = (poly1.mVertices[i].x * axis.x + poly1.mVertices[i].y * axis.y);	// - gets removed??

			if(tmp < minA)
				minA = tmp;
			else if (tmp > maxA)
				maxA = tmp;
		}

		/* correct with offset */
		tmp = poly1.getPos().x * axis.x + poly1.getPos().y * axis.y;
		minA += tmp;
		maxA += tmp;

		/* test if they intersect, if not then return false */
		if(minA > maxB){// || maxA < minB)	{
			mtv.collision = false;
			return mtv;
		}
		else
		{	
			// get the minimal depth of intersection and the correct axis
			if(maxA < maxB)		// Will have to make it work with triangles as well!
				tmpDepth = maxA - minB;
			//else if(maxA > maxB)				// HAAAX KINGGG!!!"!!!b<3333
			//	tmpDepth = maxB - minA;
			
			if(tmpDepth < minLen)	{
				
				minLen = tmpDepth;
				projection = axis;			
			}
		}
	}
	/* return minimum intersection depth */
	mtv = MTV(minLen, projection);
	mtv.collision = true;
	return mtv;
}