#include "keypad_driver.h"
#include "stm32l4xx_hal.h"
#include "main.h"


static const char keypad_map[KEYPAD_ROWS][KEYPAD_COLS] = {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};

void keypad_init(keypad_handle_t* keypad) {
  for (int i = 0; i < KEYPAD_ROWS; i++) {
    HAL_GPIO_WritePin(keypad->row_ports[i], keypad->row_pins[i], GPIO_PIN_SET);
  }
}

/**
 * @brief  Scan the keypad for a key press on the specified column pin
 * @param  keypad: Pointer to the keypad handle
 * @param  col_pin: The column pin to scan
 * @retval The character corresponding to the pressed key, or '\0' if no key is pressed
 */
char keypad_scan(keypad_handle_t* keypad, uint16_t col_pin) {
    int col_index = -1;
    for (int i = 0; i < KEYPAD_COLS; i++) {
        if (keypad->col_pins[i] == col_pin) {
            col_index = i;
            break;
        }
    }
    char key_pressed = '\0';
    for (int i = 0; i < KEYPAD_ROWS; i++) {
        // Set the current row pin low
        HAL_GPIO_WritePin(keypad->row_ports[i], keypad->row_pins[i], GPIO_PIN_RESET);

        if (HAL_GPIO_ReadPin(keypad->col_ports[col_index], keypad->col_pins[col_index]) == GPIO_PIN_RESET) {
            // If it is, we have a key press
            key_pressed = keypad_map[i][col_index];
            break;   
        }
    }
    
    
    keypad_init(keypad);
    return key_pressed;
}