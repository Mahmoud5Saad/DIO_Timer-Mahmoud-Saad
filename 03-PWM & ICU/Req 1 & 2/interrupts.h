/*
 * interrupts.h
 *
 * Created: 12/13/2019 10:30:46 PM
 *  Author: Amr
 */ 


#ifndef INTERRUPTS_H_
#define INTERRUPTS_H_


#define INT0_VECTOR								__vector_1
#define INT1_VECTOR								__vector_2
#define INT2_VECTOR								__vector_3
#define TIMER2_COMP_VECTOR						__vector_4
#define TIMER2_OVF_VECTOR						__vector_5
#define TIMER1_CAPT_VECTOR						__vector_6
#define TIMER1_COMPA_VECTOR						__vector_7
#define TIMER1_COMPB_VECTOR						__vector_8
#define TIMER1_OVF_VECTOR						__vector_9
#define TIMER0_COMP_VECTOR						__vector_10
#define TIMER0_OVF_VECTOR						__vector_11
#define SPI_STC_VECTOR							__vector_12
#define USART_RXC_VECTOR						__vector_13
#define USART_UDRE_VECTOR						__vector_14
#define USART_TXC_VECTOR						__vector_15
#define ADC_VECTOR								__vector_16
#define EE_RDY_VECTOR							__vector_17
#define ANA_COMP_VECTOR							__vector_118
#define TWI_vVECTOR								__vector_19
#define SPM_RDY_VECTOR							__vector_20

#define __INTR_ATTRS used, externally_visible

# define ISR(vector, ...)            \
void vector (void) __attribute__ ((signal,__INTR_ATTRS)) __VA_ARGS__; \
void vector (void)

#define sei()  __asm__ __volatile__ ("sei" ::: "memory")
#define cli()  __asm__ __volatile__ ("cli" ::: "memory")




#endif /* INTERRUPTS_H_ */