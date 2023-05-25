#include <stdio.h>
#include <math.h>
#include "Wall.h"

point* point_create(int x, int y){
	point* p = (point*)malloc(sizeof(point));
	p->x = (double)x;
	p->y = (double)y;
	return p;
}

segment* segment_create(point* start, point* end){
	segment* wall = (segment*)malloc(sizeof(segment));

	wall->start = start;
	wall->end = end;
	wall->orientation = GetOrientation(wall);
	return wall;
}

enum WallOrientation GetOrientation(segment* wall){
	if (wall->start->x == wall->end->x){
		return VERTICAL;
	} else {
		return HORIZONTAL;
	}
}

// determine a wall located on which side of the partition segment
enum ESegmentWallSide ClassifySegment(segment PartitionSeg, segment wall)
{
	segment* ps = segment_create(point_create(PartitionSeg.start->x, PartitionSeg.start->y), point_create(PartitionSeg.end->x, PartitionSeg.end->y));
	segment* w = segment_create(point_create(wall.start->x, wall.start->y), point_create(wall.end->x, wall.end->y));
	if(GetOrientation(ps) == VERTICAL) {
		if(GetOrientation(w) == VERTICAL) {
			if(w->start->x <= ps->start->x) {
				return SEGMENT_WALL_SIDE_FRONT;
			}
			else {
				return SEGMENT_WALL_SIDE_BEHIND;
			}
		}
		else {
			if(w->start->x <= ps->start->x && w->end->x <= ps->start->x) {
				return SEGMENT_WALL_SIDE_FRONT;
			}
			else if(w->start->x >= ps->start->x && w->end->x >= ps->start->x) {
				return SEGMENT_WALL_SIDE_BEHIND;
			}
			else {
				return SEGMENT_WALL_SIDE_STRADDLING;
			}
			return -1;
		}
	}
	else if(GetOrientation(ps) == HORIZONTAL /*&& GetOrientation(w) == VERTICAL*/) { // orientation is horizontal
		if(GetOrientation(w) == HORIZONTAL) {
			if(w->start->y <= ps->start->y) {
				return SEGMENT_WALL_SIDE_FRONT;
			}
			else {
				return SEGMENT_WALL_SIDE_BEHIND;
			}
		}
		else {
			if(w->start->y <= ps->start->y && w->end->y <= ps->start->y) {
				return SEGMENT_WALL_SIDE_FRONT;
			} 
			else if(w->start->y >= ps->start->y && w->end->y >= ps->start->y) {
				return SEGMENT_WALL_SIDE_BEHIND;
			}
			else {
				return SEGMENT_WALL_SIDE_STRADDLING;
			}
			return -1;
		}
	}
	return -1;
}

// compute the intersection point on the wall divided by the PartitionSeg
int IntersectionWalls(segment PartitionSeg, segment wall, point* out){
	int ret;
	if(wall.orientation == PartitionSeg.orientation) {
		ret = -1;
	}
	else {
		if(GetOrientation(&PartitionSeg) == VERTICAL) {
			if(((PartitionSeg.start->x >= wall.start->x && PartitionSeg.end->x <= wall.end->x) || (PartitionSeg.start->x <= wall.start->x && PartitionSeg.end->x >= wall.end->x))) {
				ret = 1;
				out->y = wall.start->y;
                out->x = PartitionSeg.start->x;
			}
			else {
				ret = -1;
			}
		}
		else if(GetOrientation(&PartitionSeg) == HORIZONTAL) {
			if (((PartitionSeg.start->y >= wall.start->y && PartitionSeg.end->y <= wall.end->y) || (PartitionSeg.start->y <= wall.start->y && PartitionSeg.end->y >= wall.end->y))) {
				ret = 1;
				out->x = wall.start->x;
				out->y = PartitionSeg.start->y;
			}
			else {
				ret = -1;
			}
		}
	}
	return ret;
}


// Output the two segments of the original wall divided by the partition segment to frontSeg and backSeg
void SplitWallSegment(segment PartitionSeg, segment wall, segment *frontSeg, segment *backSeg)
{
	point* INTERSECT = point_create(0, 0);
	segment* temp = (segment*)malloc(sizeof(segment));
	segment* temp2 = (segment*)malloc(sizeof(segment));

	int see = IntersectionWalls(PartitionSeg, wall, INTERSECT);
	if(see == -1) {
		return;
	}
	else {
		if(GetOrientation(&PartitionSeg) == VERTICAL) {
			point* front1 = point_create(wall.start->x, wall.start->y);
			point* front2 = INTERSECT;

			point* behind1 = point_create(wall.end->x, wall.end->y);
			point* behind2 = INTERSECT;
		
			if(front1->x < behind1->x) { // Make sure FrontSeg and BackSeg are positioned appropriately as start coordinates are not always < end coordinates
				frontSeg->start = behind1;
				frontSeg->end = behind2;
				backSeg->start = front1;
				backSeg->end = front2;
			}
			else {
				frontSeg->start = front1;
				frontSeg->end = front2;
				backSeg->start = behind1;
				backSeg->end = behind2;
			}

		}
		else {
			point* front1 = point_create(wall.start->x, wall.start->y);
			point* front2 = INTERSECT;

			point* behind1 = point_create(wall.end->x, wall.end->y);
			point* behind2 = INTERSECT;
			
			if(front1->y < behind1->y) { // Make sure FrontSeg and BackSeg are positioned appropriately as start coordinates are not always < end coordinates
				frontSeg->start = behind1;
				frontSeg->end = behind2;
				backSeg->start = front1;
				backSeg->end = front2;
			}
			else {
				frontSeg->start = front1;
				frontSeg->end = front2;
				backSeg->start = behind1;
				backSeg->end = behind2;
			}
		}
	}

}