#include "MouseClass.h"

void MouseClass::OnLeftPressed(int x, int y) {
	this->isLeftDown = true;
	MouseEvent me(MouseEvent::EventType::LPress, x, y);
	this->eventBuffer.push(me);
}

void MouseClass::OnRightPressed(int x, int y) {
	this->isRightDown = true;
	MouseEvent me(MouseEvent::EventType::RPress, x, y);
	this->eventBuffer.push(me);
}
void MouseClass::OnMiddlePressed(int x, int y) {
	this->isMiddleDown = true;
	MouseEvent me(MouseEvent::EventType::MPress, x, y);
	this->eventBuffer.push(me);
}

void MouseClass::OnLeftReleased(int x, int y) {
	this->isLeftDown = false;
	MouseEvent me(MouseEvent::EventType::LRelease, x, y);
	this->eventBuffer.push(me);
}

void MouseClass::OnRightReleased(int x, int y) {
	this->isRightDown = false;
	MouseEvent me(MouseEvent::EventType::RRelease, x, y);
	this->eventBuffer.push(me);
}
void MouseClass::OnMiddleReleased(int x, int y) {
	this->isMiddleDown = false;
	MouseEvent me(MouseEvent::EventType::MRelease, x, y);
	this->eventBuffer.push(me);
}

void MouseClass::OnWheelUp(int x, int y) {
	MouseEvent me(MouseEvent::EventType::WheelUp, x, y);
	this->eventBuffer.push(me);
}

void MouseClass::OnWheelDown(int x, int y) {
	MouseEvent me(MouseEvent::EventType::WheelDown, x, y);
	this->eventBuffer.push(me);
}
void MouseClass::OnMouseMove(int x, int y) {
	this->x = x;
	this->y = y;
	this->eventBuffer.push(MouseEvent(MouseEvent::EventType::Move, x, y));
}
void MouseClass::OnMouseMoveRaw(int x, int y) {
	this->eventBuffer.push(MouseEvent(MouseEvent::EventType::RAW_MOVE, x, y));
}
bool MouseClass::IsLeftDown() {
	return this->isLeftDown;

}

bool MouseClass::IsRightDown() {
	return this->isRightDown;
}

bool MouseClass::IsMiddleDown() {
	return this->isMiddleDown;
}

int MouseClass::GetX() {
	return this->x;
}
int MouseClass::GetY() {
	return this->y;
}

MousePoint MouseClass::GetPos() {
	return { this->x, this->y };
}

bool MouseClass::EventBufferIsEmpty() {
	return this->eventBuffer.empty();
}

MouseEvent MouseClass::ReadEvent() {
	if (this->eventBuffer.empty()) {
		return MouseEvent();
	}
	MouseEvent e = this->eventBuffer.front();
	this->eventBuffer.pop();
	return e;
}