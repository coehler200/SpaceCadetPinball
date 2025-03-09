#include "data.h"
#include "score.h"
#include "TBall.h"
#include "TFlipper.h"
#include "TPinballComponent.h"

void Data::printData(TPinballTable* table)
{
	int score = table->CurScoreStruct->Score;

	TBall* ball = table->BallList[0];
	float ballPosX = ball->Position.X;
	float ballPosY = ball->Position.Y;
	float ballVelX = ball->Direction.X;
	float ballVelY = ball->Direction.Y;

	TFlipper* flipperL = table->FlipperL;

	flipperL->Message(MessageCode::TFlipperExtend, 0); // Can be used to control flippers

	printf("X: %f Y: %f Vx: %f Vy: %f Score: %d\n", ballPosX, ballPosY, ballVelX, ballVelY, score);
}
