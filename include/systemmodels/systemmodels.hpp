/**
  ******************************************************************************
  * @file    systemmodels/systemmodels.hpp
  * @author  RBRO/PJ-IU
  * @version V1.0.0
  * @date    day-month-2017
  * @brief   This file contains the class declarations for system model.
  ******************************************************************************
 */

/* Inclusion guard */
#ifndef SYSTEM_MODELS_HPP
#define SYSTEM_MODELS_HPP

#include <linalg/linalg.h>

// Discrete System Models
namespace systemmodels{
    //Linear time variant models
    namespace lti{
        namespace siso{
           /**
            * @brief Transfer function in z-domain. 
            * 
            * The transfer function express by z^{-1} and is represented by ratio of two polynomials. 
            * 
            * @tparam T The type of the coefficients
            * @tparam NNum The order of the polynomial in nominator 
            * @tparam NDen The order of the polynomial in denominator
            */
            template <class T,uint32_t NNum,uint32_t NDen>
            class CDiscreteTransferFunction{
                public:
                    using CDenType          =   linalg::CMatrix<T,NDen,1>; // Type of the full denominator coefficients
                    using CDenModType       =   linalg::CMatrix<T,NDen-1,1>; // Type of the denominator coefficients without the first coefficient
                    using CNumType          =   linalg::CMatrix<T,NNum,1>; // Type of the full nominator coefficients
                    
                    using CInputMem         =   linalg::CMatrix<T,1,NNum>; // Type of previous input value memory
                    using COutputMem        =   linalg::CMatrix<T,1,NDen-1>; // Type of previous output value memory
                    /* Constructor */
                    CDiscreteTransferFunction();

                    CDiscreteTransferFunction(const CNumType& f_num,const CDenType& f_den);

                    /* Clear memory */
                    void clearMemmory();
                    /* Shift memory */
                    template<uint32_t N>
                    void shiftMemory(linalg::CMatrix<T,1,N>& f_mem);
                    /* Applying the transfer function on the next signal value */
                    T operator()(const T& f_input);
                    /* Setting the nominator coefficients */
                    void setNum(const CNumType& f_num);
                    /* Setting the denominator coefficients */
                    void setDen(const CDenType& f_den);

                    /* Getting the numerator coefficients */
                    const CNumType& getNum();
                    /* Getting the denominator coefficients without the first coefficient*/
                    const CDenModType& getDen();
                    /* Getting the first denominator coefficients. General normalized to 1 */
                    float getDenCurrent();
                    /* Getting the last calculated output */
                    T getOutput();

                private:
                    /* nominator coefficients */
                    CNumType    m_num;
                    /* denominator coefficients */
                    CDenModType    m_den;
                    T              m_denCoef; // The first coefficient in polynomials, it mustn't be zero value. 
                    /* input memory */
                    CInputMem      m_memInput;
                    /* output memory */
                    COutputMem     m_memOutput;
            }; // class CDiscreteTransferFunction
        }; // namespace siso
        namespace mimo{
            /**
             * @brief Class for State Space Model
             * 
             * @tparam T        type of the variables, 
             * @tparam NA       number of states variable
             * @tparam NB       number of control variable
             * @tparam NC       number of observation variable  
             */
            template <class T, uint32_t NA, uint32_t NB, uint32_t NC>
            class CSSModel
            {
                public:
                    using CStateType = linalg::CColVector<T,NA>; // X - state variable type
                    using CStateTransitionType = linalg::CMatrix<T,NA,NA>; // A state-state trans. model type
                    using CControlType = linalg::CColVector<T,NB>; // U - control variable type
                    using CMeasurementType = linalg::CColVector<T,NC>; // Y - observation (measurement) variable type
                    using CInputMatrixType = linalg::CMatrix<T,NA,NB>; // B - control-state trans. model type
                    using CMeasurementMatrixType = linalg::CMatrix<T,NC,NA>; // C - state-observation trans. model type
                    using CDirectTransferMatrixType = linalg::CMatrix<T,NC,NB>; // D - control-observation trans. model type
                    /* Constructor */
                    CSSModel(
                        const CStateTransitionType& f_stateTransitionMatrix,
                        const CInputMatrixType& f_inputMatrix,
                        const CMeasurementMatrixType& f_measurementMatrix);
                    /* Constructor */
                    CSSModel(
                        const CStateTransitionType& f_stateTransitionMatrix,
                        const CInputMatrixType& f_inputMatrix,
                        const CMeasurementMatrixType& f_measurementMatrix,
                        const CDirectTransferMatrixType& f_directTransferMatrix);
                    /* Constructor */
                    CSSModel(
                        const CStateTransitionType& f_stateTransitionMatrix,
                        const CInputMatrixType& f_inputMatrix,
                        const CMeasurementMatrixType& f_measurementMatrix,
                        const CDirectTransferMatrixType& f_directTransferMatrix,
                        const CStateType& f_state);

                    /* Get state method */
                    const CStateType& state() const {return m_stateVector;} 
                    CStateType& state() {return m_stateVector;} 

                    /* Operator */
                    CMeasurementType operator()(const CControlType& f_inputVector);
                    /* Update state */
                    void updateState(const CControlType& f_inputVector);
                    /* Get output */
                    CMeasurementType getOutput(const CControlType& f_inputVector);

                private:
                    CSSModel() {}
                    /* state vector */
                    CStateType m_stateVector;
                    /* state transition matrix */
                    CStateTransitionType m_stateTransitionMatrix;
                    /* input matrix */
                    CInputMatrixType m_inputMatrix;
                    /* measurement (observation) matrix */
                    CMeasurementMatrixType m_measurementMatrix;
                    /* direct transfer matrix */
                    CDirectTransferMatrixType m_directTransferMatrix;
            }; // class CSSModel
        }; //namespace mimo
    };
    // Nonlinea time invariant models
    namespace nlti{
        //Multi-input and multi-output 
        namespace mimo{
            // Discrete time system model of a system nonlinear time invariant with multi-input and multi-output
            //  T       -variable type
            //  NA      -number of control
            //  NB      -number of states
            //  NC      -number of outputs
            template <class T,uint32_t NA, uint32_t NB,uint32_t NC>
            class CDiscreteTimeSystemModel{
                public:
                    using CStatesType           =   linalg::CMatrix<T,NB,1>;
                    using CControlType            =   linalg::CMatrix<T,NA,1>;
                    using CObservationType           =   linalg::CMatrix<T,NC,1>;
                    /* Constructor */
                    CDiscreteTimeSystemModel(const double           f_dt);//:m_states(),m_dt(f_dt){}
                    /* Constructor */
                    CDiscreteTimeSystemModel(const CStatesType&     f_states
                                            ,const double           f_dt);
                    //State transition model
                    //Calculate the system state depending on input, after calculation the relevant class members need to be syncronized.
                    virtual CStatesType update(const CControlType&       f_input)=0;
                    //State observation model
                    //Calculate the system output depending on input, after calculation the relevant class members need to be syncronized.
                    virtual CObservationType calculateOutput(const CControlType&  f_input)=0;
                    // GETTERS
                    // The method returns the current system states
                    CStatesType         getStates();//{return m_states;}
                    // The method returns the current system output
                    CObservationType         getOutput(){return m_outputs;}
                    float               getTimeStep(){return m_dt;}
                    // SETTERS
                    void                setStates(const CStatesType& f_states){
                        m_states=f_states;
                    }
                protected:
                    // States
                    CStatesType                         m_states;
                    // Output
                    CObservationType                         m_outputs;
                    // Time step
                    const double                        m_dt;
                private:        
            }; // class CDiscreteTimeSystemModel

        }; //namespace mimo
    }; //namespace nlti
}; // namespace systemmodels

#include "systemmodels.tpp"

#endif