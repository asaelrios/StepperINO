#include <stdint.h>

class IndexStepper
{
    public:

        typedef enum
        {
            DIR_CCW = 0,            /*!< Counter-Clockwise>*/
            DIR_CW = 1              /*!< Clockwise>*/
        } Direction;


        typedef struct StepMode
        {
            int             CW_singleCoil[4][4] = 
                                    {
                                        {1,0,0,0},
                                        {0,0,0,1},
                                        {0,0,1,0},
                                        {0,1,0,0}
                                    };
            int             CW_doubleCoil[4][4] =
                                    {
                                        {1,1,0,0},
                                        {1,0,0,1},
                                        {0,0,1,1},
                                        {0,1,1,0}
                                    };
            int             CW_halfStep[8][4] = 
                                    {
                                        {1,0,0,0},
                                        {1,0,0,1},
                                        {0,0,0,1},
                                        {0,0,1,1},
                                        {0,0,1,0},
                                        {0,1,1,0},
                                        {0,1,0,0},
                                        {1,1,0,0}
                                    }; 
            int             CCW_singleCoil[4][4] = 
                                    {
                                        {1,0,0,0},
                                        {0,1,0,0},
                                        {0,0,1,0},
                                        {0,0,0,1}
                                    };
            int             CCW_doubleCoil[4][4] =
                                    {
                                        {1,1,0,0},
                                        {0,1,1,0},
                                        {0,0,1,1},
                                        {1,0,0,1}
                                    };
            int             CCW_halfStep[8][4] = 
                                    {
                                        {1,0,0,0},
                                        {1,1,0,0},
                                        {0,1,0,0},
                                        {0,1,1,0},
                                        {0,0,1,0},
                                        {0,0,1,1},
                                        {0,0,0,1},
                                        {1,0,0,1}
                                    };  
        } StepMode;           


/*
 *  GET & SET methods
 */


//  Pinout GET & SET
        void                setPinout(uint8_t pinout[]);

        uint8_t             getPinout();

//  Degrees GET & SET
        void                setDegrees(float degrees);

        float               getDegrees();

//  Current Position GET & SET
        void                setCurrentPosition(float degrees);

        float               getCurrentPosition();

//  Steps GET & SET
        void                setSteps(int steps);

        int                 getSteps();

//  Increment GET & SET
        void                setIncrementPosition(int steps);

        int                 getIncrementPosition();

//  Last Position GET & SET
        void                setLastPosition(float position);

        float                 getLastPosition();

//  Direction GET & SET
        void                setDirection(int direction);

        int                 getDirection();

/*
 *  STEPPER OPERATION METHODS
 */  

        int                 doSteps(int steps, int stepmode[4][4]);


        int                 degreesToSteps(float degrees);


        float               currentPosition();


        void                targetPosition(float degrees);


        void                incrementPosition(int degrees);


        void                lastPosition();


        void                zeroPosition();


        bool                initStep(uint8_t pinout[]);


        bool                run_cycle(float degrees, char mode);


        bool                stop_cycle();

/*
 *  OPERATION VARIABLES
 */
    private:


        uint8_t             _pinout[4];


        float               _degrees;


        int                 _steps;


        int                 _direction;


        float               _currentPos;


        float               _targetPos;


        int               _incrementPos;


        float               _lastPos;


        float               _zeroPos;


        bool                _run;


        bool                _stop;


};
