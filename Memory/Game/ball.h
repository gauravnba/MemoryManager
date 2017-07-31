class BallC : public ObjectC
{
public:
    BallC();
	BallC(float_t initPosX, float_t initPosY, float_t initVelX, float_t initVelY, int id);
    ~BallC();
	void setBallColor(long color) {mBallColor = color;};
	void setBallColor(unsigned char r, unsigned char g, unsigned char b);

    long getBallColor() {return mBallColor;};
    void update(DWORD milliseconds);
    virtual void render();
	void DoCollisions();
    void disable() {mEnabled = false;};
    void enable() {mEnabled = true;};
    void setRadius(float radius) {mCollInfo.param1 = radius;mRadius = radius;};
	void setRandomColor();
	void reduceVelocity(float_t factor);
	void addFrameSinceCreation() {mFramesSinceCreation++;};
	uint32_t getAge() {return mFramesSinceCreation;};
	uint32_t getDataBufferSize() {return mDataBufferSize;};
private:
	void CollideField();

	long		mBallColor;
    bool		mEnabled;
    float		mRadius;
	bool		mAtRest;
	uint32_t	mFramesSinceCreation;

	void		*mDataBuffer;
	uint32_t	mDataBufferSize;
};