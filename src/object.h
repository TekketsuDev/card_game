#ifndef OBJECT_H_
#define OBJECT_H_

#include "raylib.h"

typedef struct Object {

  Vector2 position;
  Vector2 size;
  Color color;
  bool draggable;
  Vector2 dragOffset;
  Vector2 prevPos;
  int handSlot;
} Object;

Object CreateObject(Vector2 position, Vector2 size, Color color) {
  return (Object) {
    .position = position,
    .size = size,
    .color = color,
    .draggable = draggable,
    .draggOffset = dragOffset,
    .prevPos = prevPos,
    .handSlot = .handSlot
    //Avoid order initialization
  };
}
void DrawObject (Object object) {
  DrawRectangleV(object.position, object.size, object.color);
}
#endif
