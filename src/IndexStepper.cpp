#include "indexStepper.h"
#include "Arduino.h"


/*
 *  PINOUT SET & GET
 */

/*!< Pinout Setter >*/
void IndexStepper :: setPinout(uint8_t pinout[])
{
    _pinout[0] = pinout[0];
    _pinout[1] = pinout[1];
    _pinout[2] = pinout[2];
    _pinout[3] = pinout[3];

    pinMode(_pinout[0], OUTPUT);
    pinMode(_pinout[1], OUTPUT);
    pinMode(_pinout[2], OUTPUT);
    pinMode(_pinout[3], OUTPUT);
}

/*!< Pinout Getter >*/
uint8_t IndexStepper :: getPinout()
{
    return _pinout[0];
}

/*
 *  DEGREES SET & GET
 */

/*!< Degrees Setter >*/
void IndexStepper :: setDegrees(float degrees)
{
    _degrees = degrees;
}

/*!< Degrees Getter >*/
float IndexStepper :: getDegrees()
{
    return _degrees;
}

/*
 *  CURRENT POSITION SET & GET
 */

/*!< Current Position Setter >*/
void IndexStepper :: setCurrentPosition(float degrees)
{
    _currentPos = degrees;
}

/*!< Current Position Getter >*/
float IndexStepper :: getCurrentPosition()
{
    return _currentPos;
}

/*
 *  STEPS COUNTER SET & GET
 */

/*!< Steps Counter Setter >*/
void IndexStepper :: setSteps(int steps)
{
    _steps = steps;    
}

/*!< Steps Counter Getter >*/
int IndexStepper :: getSteps()
{
    return _steps;
}

/*
 *  INCREMENT POSITION SET & GET
 */

/*!< Increment Setter >*/
void IndexStepper :: setIncrementPosition(int steps)
{
    _incrementPos = steps;    
}

/*!< Increment Getter >*/
int IndexStepper :: getIncrementPosition()
{
    return _incrementPos;
}

/*
 *  LAST POSITION SET & GET
 */

/*!< Last Position Setter >*/
void IndexStepper :: setLastPosition(float position)
{
    _lastPos = position;
}

/*!< Direction Getter >*/
float IndexStepper :: getLastPosition()
{
    return _lastPos;
}

/*
 *  DIRECTION SET & GET
 */

/*!< Direction Setter >*/
void IndexStepper :: setDirection(int direction)
{
    _direction = direction;
}

/*!< Direction Getter >*/
int IndexStepper :: getDirection()
{
    return _direction;
}

/****************************************************************************
 * @fn					- doSteps
 *
 * @brief				- Steps for stepper
 *
 * @param[in]			- Steps to reach position
 *
 * @param[in]           - Steps mode
 * 
 * @return				- Void
 *
 * @Note				- None
 * 
 */

int IndexStepper :: doSteps(int steps, int stepmode[4][4])
{
    /*
    * Double Coil Steps
    * 36:1 Reduction
    * StepMode doubleCoil rutine has 4 Steps
    * Steps should be multiplied to match transmission reduction
    * eg. (36:1) = 4 steps * 9 times
    */

   int stepsToPos = steps * 9;

   for(int stp=0; stp<stepsToPos; stp++)
   {
       for(int coil=0; coil<4; coil++)
       {
           digitalWrite(_pinout[0], stepmode[coil][0]);
           digitalWrite(_pinout[1], stepmode[coil][1]);
           digitalWrite(_pinout[2], stepmode[coil][2]);
           digitalWrite(_pinout[3], stepmode[coil][3]);
       }
   }
   return stepsToPos;
}

/****************************************************************************
 * @fn					- degreesToSteps
 *
 * @brief				- Steps calculation from degrees input
 *
 * @param[in]			- Degrees to reach position
 *
 * @return				- Steps to reach position
 *
 * @Note				- This steps are calculated for 28BYJ-48 Stepper 
 *                      - in Double Coil operation,
 *                      - hence 2048 steps makes ONE complete turn.
 *                      - For Half-Step, changes are required.
 *                      - Half-Step: 4096                       
 * 
 */

int IndexStepper :: degreesToSteps(float degrees)
{
    float travel = (degrees*10000)/360;
    int travel_int = round(travel);
    float pre_steps = (travel_int)*2048; // 2048 Steps for complete turn
    float steps = pre_steps/10000;
    return steps;
}

/****************************************************************************
 * @fn					- currentPosition
 *
 * @brief				- Current position in degrees
 *
 * @param[in]			- None
 *
 * @return				- Current Position
 *
 * @Note				- For use inside class use getCurrentPosition() instead
 * 
 */

float IndexStepper :: currentPosition()
{
    return _currentPos;
}

/****************************************************************************
 * @fn					- targetPosition
 *
 * @brief				- Stepper advance to especified position
 *
 * @param[in]			- Target position in degrees
 *
 * @return				- Void
 *
 * @Note				- None
 * 
 */

void IndexStepper :: targetPosition(float degrees)
{
    int steps = 0;
    int step_cnt;
    StepMode stepmode;

    if(degrees >= _degrees)
    {
        degrees += _degrees;
        steps = IndexStepper :: degreesToSteps(degrees);
        setDirection(1);
    }else if (degrees < _degrees)
    {
        degrees -= _degrees;
        steps = IndexStepper :: degreesToSteps(degrees);
        setDirection(0);
    }

    if (_direction == DIR_CW)
    {
        step_cnt = IndexStepper :: doSteps(steps, stepmode.CW_doubleCoil);
        step_cnt += _steps;
    }else if (_direction == DIR_CCW)
    {
        step_cnt = IndexStepper :: doSteps(steps, stepmode.CCW_doubleCoil);
        step_cnt = _steps - step_cnt;
    }

    IndexStepper :: setLastPosition(_currentPos);
    IndexStepper :: setSteps(step_cnt);
    IndexStepper :: setDegrees(degrees);
    IndexStepper :: setCurrentPosition(degrees);
}

/****************************************************************************
 * @fn					- incrementPosition
 *
 * @brief				- Stepper advance with given increment
 *
 * @param[in]			- Degrees to advance
 *
 * @return				- Void
 *
 * @Note				- None
 * 
 */

void IndexStepper :: incrementPosition(int degrees)
{
    int steps = IndexStepper :: degreesToSteps(degrees);
    int step_cnt;
    float newDegrees = degrees;
    IndexStepper :: setIncrementPosition(steps);
    StepMode stepmode;

        if(degrees >= _degrees)
    {
        degrees += _degrees;
        setDirection(1);
    }else if (degrees < _degrees)
    {
        degrees -= _degrees;
        setDirection(0);
    }

    if (_direction == DIR_CW)
    {
        step_cnt = IndexStepper :: doSteps(_incrementPos, stepmode.CW_doubleCoil);
        step_cnt += _steps;
    }else if (_direction == DIR_CCW)
    {
        step_cnt = IndexStepper :: doSteps(_incrementPos, stepmode.CCW_doubleCoil);
        step_cnt = _steps - step_cnt;
    }    

    IndexStepper :: setLastPosition(_currentPos);
    IndexStepper :: setSteps(step_cnt);
    IndexStepper :: setDegrees(newDegrees);
    IndexStepper :: setCurrentPosition(degrees);
}

/****************************************************************************
 * @fn					- lastPosition
 *
 * @brief				- Stepper return to last position
 *
 * @param[in]			- None
 *
 * @return				- Void
 *
 * @Note				- None
 * 
 */

void IndexStepper :: lastPosition()
{
    int step_cnt;
    StepMode stepmode;

    //Normal logic inverted
    if (_direction == DIR_CW)
    {
    step_cnt = IndexStepper :: doSteps(_incrementPos, stepmode.CCW_doubleCoil);
    step_cnt = _steps - step_cnt; 
    }else if (_direction == DIR_CCW)
    {
    step_cnt = IndexStepper :: doSteps(_incrementPos, stepmode.CW_doubleCoil);
    step_cnt += _steps;
    } 

    IndexStepper :: setLastPosition(_currentPos);
    IndexStepper :: setSteps(step_cnt);
    IndexStepper :: setCurrentPosition(_lastPos);   
}

/****************************************************************************
 * @fn					- zeroPosition
 *
 * @brief				- Stepper return to zero position
 *
 * @param[in]			- None
 *
 * @return				- Void
 *
 * @Note				- None
 * 
 */

void IndexStepper :: zeroPosition()
{
    int step_cnt;
    int zerosteps = 0;
    float zerodegrees = 0.0;
    StepMode stepmode;

    if(_currentPos >= 0)
    {
        step_cnt = IndexStepper :: doSteps(_steps, stepmode.CCW_doubleCoil);
    }
    else if(_currentPos < 0)
    {
        step_cnt = IndexStepper :: doSteps(_steps, stepmode.CW_doubleCoil);
    }

    IndexStepper :: setLastPosition(_currentPos);
    IndexStepper :: setIncrementPosition(_steps);
    IndexStepper :: setCurrentPosition(zerodegrees);
    IndexStepper :: setSteps(zerosteps);
}

/****************************************************************************
 * @fn					- initStep
 *
 * @brief				- Stepper initialization whit pinout
 *
 * @param[in]			- Digital pins to use
 *
 * @return				- TRUE
 *
 * @Note				- None
 * 
 */

bool IndexStepper :: initStep(uint8_t pinout[4])
{
    // Initializing variables to zero
    int zerosteps = 0;
    float zerodegrees = 0.0;

    IndexStepper :: setCurrentPosition(zerodegrees);
    IndexStepper :: setDegrees(zerodegrees);
    IndexStepper :: setSteps(zerosteps);
    IndexStepper :: setPinout(pinout);

    return true;
}


/****************************************************************************
 * @fn					- run_cycle
 *
 * @brief				- Operation mode selection and degrees setting
 *
 * @param[in]			- Degrees to advance
 *
 * @param[in]           - Operation mode
 * 
 * @return				- Run cycle state
 *
 * @Note				- 4 Options, Target, Increment, Last, Zero Positions
 *
 */

bool IndexStepper :: run_cycle(float degrees, char mode)
{
    // Transition mode
    switch (mode)
    {
    /*!<Target Position Mode>*/
    case 't':
        IndexStepper :: targetPosition(degrees);
        return true;
        break;

    /*!<Increment Position Mode>*/
    case 'i':
        IndexStepper :: incrementPosition(degrees);
        return true;
        break;

    /*!<Last Position Mode>*/
    case 'l':
        IndexStepper :: lastPosition();
        return true;
        break;

    /*!<Zero Position Mode>*/
    case 'z':
        IndexStepper :: zeroPosition();
        return true;
        break;

    /*!<Void Position Mode Intent>*/
    default:
        return false;
        break;
    }
}

/****************************************************************************
 * @fn					- stop_cycle
 *
 * @brief				- Stop operation
 *
 * @return				- Stop cycle state
 *
 * @Note				- None
 *
 */

bool IndexStepper :: stop_cycle()
{
    digitalWrite(_pinout[0], 0);
    digitalWrite(_pinout[1], 0);
    digitalWrite(_pinout[2], 0);
    digitalWrite(_pinout[3], 0);
    return true;
}
