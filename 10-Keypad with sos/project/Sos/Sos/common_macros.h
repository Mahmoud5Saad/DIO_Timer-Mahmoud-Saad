/*
 * common_macros.h
 *
 *  Created on: Dec 22, 2019
 *      Author: Khalaf & Zayed
 */

#ifndef SPRINTS_ZAYED_AVR_STATICDESIGN_SRC_MCAL_COMMON_MACROS_H_
#define SPRINTS_ZAYED_AVR_STATICDESIGN_SRC_MCAL_COMMON_MACROS_H_


/************************************************************************/
/*			       			BIT_MATH defines       	                    */
/************************************************************************/



/* Set VARiable's Certin BIT Low */

#define CLR_BIT(VAR,BIT_NB) 		(VAR)&=~(BIT_NB)


/* Assign VARiable's Certin BIT by a Certin VALue */

#define ASSIGN_BIT(VAR,BIT_NB,VAL)  (VAR)=((VAR&(~(BIT_NB)))|(VAL&BIT_NB))

/* Get VARiable's Certin BIT Value */

#define GET_BIT(VAR,BIT_NB) 		(((VAR)&(BIT_NB))/(BIT_NB))

/* macro to set a certain BIT inside a register to 1 */
#define CLEAR_BIT(REGISTER,BIT) ((REGISTER)&=(~(1<<(BIT))))

/* macro to clear a certain BIT inside a register (put its value to be zero) */
#define SET_BIT(REGISTER,BIT)  ((REGISTER)|=(1<<(BIT)))

/*macro to toggle a bit */
#define TOGGLE_BIT(REGISTER,BIT) ((REGISTER)^=(1<<(BIT)))

/* macro to check if bit is set (equal to 1) */
#define BIT_IS_SET(REGISTER,BIT) ((REGISTER)&(1<<(BIT)))

/* macro to check if a certain bit inside a register is cleared */
#define BIT_IS_CLEAR(REGISTER,BIT) (!((REGISTER)&(1<<(BIT))))

/* this macro is to rotate right  */
#define ROR(REGISTER,num) ((REGISTER)=(((REGISTER)>>(num))|((REGISTER)<<(8-(num)))))

/* rotate bit of the register in the left direction */
#define ROL(REGISTER,num) ((REGISTER)=(((REGISTER)<<(num))|((REGISTER)>>(8-(num)))))




#endif /* SPRINTS_ZAYED_AVR_STATICDESIGN_SRC_MCAL_COMMON_MACROS_H_ */
