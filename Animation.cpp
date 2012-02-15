#include "Animation.h"
#include "Graphics.h"

Animation::Animation(int frameWidth, int frameHeight, float timePerFrame, int totalFrames, int colums)
{
	mFrameWidth = frameWidth;
	mFrameHeight = frameHeight;
	mTimePerFrame = timePerFrame;
	mTotalFrames = totalFrames;
	mColums = colums;
	mDeltaSum = 0;
	mCurrentFrame = 0;
	mPaused = false;
	mOffset = 0;
}
	
Animation::~Animation()
{
	
}

void Animation::animate(float dt)
{
	if(!mPaused)
	{
		// Perform animation, calculate frame
		if(mDeltaSum >= mTimePerFrame)	{
			setFrame(mCurrentFrame + 1);
			if(mCurrentFrame >= mTotalFrames)
				setFrame(0);
			
			mDeltaSum = 0;
		}
		else
			mDeltaSum += dt;

		// Calculate the source Rect
		// Calculate frame location
		int fx = ((mCurrentFrame + mOffset) % mColums) * mFrameWidth;
		int fy = ((mCurrentFrame + mOffset) / mColums) * mFrameHeight;

		mSourceRect = Rect((float)fx, (float)fx + mFrameWidth, (float)fy, (float)fy + mFrameHeight);
	}
}

void Animation::setFrame(int currentFrame)
{
	mCurrentFrame = currentFrame;

	// Calculate source rect
	int fx = (mCurrentFrame % mColums) * mFrameWidth;
	int fy = (mCurrentFrame / mColums) * mFrameHeight;

	mSourceRect = Rect((float)fx, (float)fx + mFrameWidth, (float)fy, (float)fy + mFrameHeight);

	/*mSourceRect.left = fx; mSourceRect.top = fy;
	mSourceRect.right = fx + mFrameWidth; mSourceRect.bottom = fy + mFrameHeight;*/
}