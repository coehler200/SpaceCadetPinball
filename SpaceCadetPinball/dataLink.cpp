#include "dataLink.h"
#include <windows.h> 
#include "score.h"
#include "TBall.h"
#include "TFlipper.h"
#include "TFlipperEdge.h"
#include "TPinballComponent.h"
#include "TPlunger.h"


HANDLE hPipe;
TPinballTable* table;
char bufIn[1024];
char bufOut[1024];
DWORD dwRead;
DWORD dwWrite;
unsigned long int frame;

DataLink::DataLink()
{
	table = nullptr;
	frame = 0;
}

DataLink::DataLink(TPinballTable* t)
{
	table = t;
	frame = 0;
}

DataLink::~DataLink()
{
	DisconnectNamedPipe(hPipe);
}

void DataLink::setTable(TPinballTable* t)
{
	table = t;
}

bool DataLink::init()
{
	hPipe = CreateNamedPipe(TEXT("\\\\.\\pipe\\Pinball"),
		PIPE_ACCESS_DUPLEX,
		PIPE_TYPE_MESSAGE | PIPE_READMODE_MESSAGE | PIPE_WAIT,
		1,
		1024 * 16,
		1024 * 16,
		NMPWAIT_NOWAIT,
		NULL);

	printf("Attempting Pipe Connection...\n");

	while (ConnectNamedPipe(hPipe, NULL) == FALSE) {
		printf("Awaiting Connection...\n");
	}

	printf("Pipe connected!\n");

	return TRUE;
}

void DataLink::update(float time)
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

	sprintf(bufOut, "{"
		"\"frame\": %lu,"
		"\"ballPosX\": %f,"
		"\"ballPosY\": %f,"
		"\"ballVelX\": %f,"
		"\"ballVelY\": %f,"
		"\"flipperLAngle\": %f,"
		"\"flipperRAngle\": %f,"
		"\"score\": %d,"
		"\"inDrain\": %d"
	"}", frame, ballPosX, ballPosY, ballVelX, ballVelY, flipperLAngle, flipperRAngle, score, ballInDrainFlag);

	WriteFile(hPipe, bufOut, strlen(bufOut), &dwWrite, NULL);

	readIncomingAndAct(time);

	frame++;
}

void DataLink::readIncomingAndAct(float time)
{
	// Read action. One of "flipperL", "flipperR", "plunger", "reset", "none"
	ReadFile(hPipe, bufIn, sizeof(bufIn) - 1, &dwRead, NULL);
	bufIn[dwRead] = '\0';
	
	if (strcmp(bufIn, "flipperL") == 0)
	{
		table->Message(MessageCode::LeftFlipperInputPressed, time);
		table->Message(MessageCode::RightFlipperInputReleased, time);
		table->Message(MessageCode::PlungerInputReleased, time);
		printf("flipperL\n");
	}
	else if (strcmp(bufIn, "flipperR") == 0)
	{
		table->Message(MessageCode::LeftFlipperInputReleased, time);
		table->Message(MessageCode::RightFlipperInputPressed, time);
		table->Message(MessageCode::PlungerInputReleased, time);
		printf("flipperR\n");
	}
	else if (strcmp(bufIn, "plunger") == 0)
	{
		table->Message(MessageCode::LeftFlipperInputReleased, time);
		table->Message(MessageCode::RightFlipperInputReleased, time);
		table->Message(MessageCode::PlungerInputPressed, time);
		printf("plunger\n");
	}
	else if (strcmp(bufIn, "resetBall") == 0)
	{
		table->Message(MessageCode::LeftFlipperInputReleased, time);
		table->Message(MessageCode::RightFlipperInputReleased, time);
		table->Message(MessageCode::PlungerInputReleased, time);
		for (auto ball : table->BallList)
		{
			if (ball->ActiveFlag)
			{
				ball->Position = vector3(0, -8);
				ball->Direction = vector3(0, 0);
			}
;		}
		printf("resetBall\n");
	}
	else if (strcmp(bufIn, "reset") == 0)
	{
		table->Message(MessageCode::NewGame, time);
		frame = 0;
		printf("reset\n");
	}
	else
	{
		table->Message(MessageCode::LeftFlipperInputReleased, time);
		table->Message(MessageCode::RightFlipperInputReleased, time);
		table->Message(MessageCode::PlungerInputReleased, time);
		printf("none\n");
	}
}
