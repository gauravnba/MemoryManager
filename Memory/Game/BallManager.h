class BallManagerC
{
public:
	static BallManagerC	*CreateInstance();
	static BallManagerC	*GetInstance() {return sInstance;};
	~BallManagerC(){};
	
	int32_t		getNumBalls() {return mBallList.Size();};
	void		init();
	void		shutdown();
	void		updateBalls(DWORD milliseconds);
	void		renderBalls();
	uint32_t	getOldestBallsAge(int *id);
	uint32_t	getTotalDataBufferSize();


private:
	float_t						getCreateProbability();
	void						handleCreationDeletion();
	void						createBall();
	void						deleteBall();
	BallC*						getRandomBall();

    int                         mCreateID;

	SList<BallC *>				mBallList;
	static BallManagerC			*sInstance;
	static const int32_t		MAX_BALLS = 500;
	BallManagerC(){};
};