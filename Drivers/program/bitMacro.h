/*
 * bitMacro.h
 *
 * Created: 02.11.2019 12:04:47
 *  Author: felixas
 */ 


#ifndef BITMACRO_H_
#define BITMACRO_H_

#define set_bit( reg, bit ) (reg |= (1 << bit))
#define clear_bit( reg, bit ) (reg &= ~(1 << bit))
#define test_bit( reg, bit ) (reg & (1 << bit))
#define loop_until_bit_is_set( reg, bit ) while( !test_bit( reg, bit ) )
#define loop_until_bit_is_clear( reg, bit ) while( test_bit( reg, bit ) )



#endif /* BITMACRO_H_ */