#ifndef FLOEKR2D_H
#define FLOEKR2D_H

// core::collision
#include <Floekr2d/private/f2Collision.h>
// Floekr2d::
#include <Floekr2d/public/f2Space.h>

#include <Floekr2d/public/f2Body.h>
// shape
#include <Floekr2d/public/shape/f2CircleShape.h>

#include <Floekr2d/public/shape/f2PolygonShape.h>

// timer
#include <Floekr2d/protected/timer/f2Timer.h>

// plugin
#ifdef F2_USEPLUGIN
// 并非内部集成
#include <Floekr2d/plugin/f2Renderer.h>
#endif

#endif