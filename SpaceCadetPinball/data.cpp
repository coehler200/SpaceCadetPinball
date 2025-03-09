#include "data.h"
#include "score.h"
#include "TBall.h"
#include "TFlipper.h"
#include "TFlipperEdge.h"
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
	TFlipper* flipperR = table->FlipperR;
	float flipperLAngle = flipperL->FlipperEdge->CurrentAngle;
	float flipperRAngle = flipperR->FlipperEdge->CurrentAngle;

	int ballInDrainFlag = table->BallInDrainFlag;

	flipperL->Message(MessageCode::TFlipperExtend, 0); // Can be used to control flippers
	// flipperL->Message(MessageCode::TFlipperRetract, 0);
	// flipperR->Message(MessageCode::TFlipperExtend, 0);

	//printf("X: %f Y: %f Vx: %f Vy: %f Score: %d\n", ballPosX, ballPosY, ballVelX, ballVelY, score);
	printf("%d %f\n", ballInDrainFlag, flipperLAngle);
}
