#include "carousel.h"
#include "utf8.h"

ASSERT_COMMUNITY_MODULES_MIN_API_VERSION(0,1,0);

#ifndef CAROUSEL_MAX
#define CAROUSEL_MAX 25
#endif
int carousel = -1;
int carousel_index = 0;

bool process_record_carousel(uint16_t keycode, keyrecord_t *record) {
    // if a carousel is already selected
    if(carousel >= 0 && carousel < CAROUSEL_MAX){
        int dir = 0;
        if(keycode == CSL_L && record->event.pressed){
            dir = -1; 
        }
        else if(keycode == CSL_R && record->event.pressed){
            dir = +1;
        }
        // check that we are scrolling the carousel        
        if(dir != 0){
            // find the next character in the carousel
            int32_t code = -1;
            while(code == -1){
                carousel_index+=dir; 
                // if we're at the start and scrolling left find the last character
                if(carousel_index < 0){ 
                    // start at the beginning 
                    carousel_index = 0;
                    // and keep going until we see the end of string marker \0
                    while(carousels[carousel].options[carousel_index]!= '\0'){
                        carousel_index++;
                    }
                    // since \0 is after the last character going back one gets us the last character
                    carousel_index--;
                } // if we've gone past the end then loop back to the start
                if(carousels[carousel].options[carousel_index] == '\0'){
                    carousel_index = 0;
                }
                // grab the character from the carousel and save it's codepoint into code
                decode_utf8(&carousels[carousel].options[carousel_index],&code);
            }  
            
            // delete the last typed character by sending backspace
            tap_code16(KC_BSPC);
            // debugging, converts the carousel index to the equivalent character
            //send_char(48+carousel_index);
            char c = carousels[carousel].options[carousel_index];
            // debugging, prints the selected character's code
            // char s[10];
            // sprintf(s,"%li",code);
            // send_string(s);

            // this is a hack to ensure that the character is ascii
            if((char)(int)code == c){
                send_char(c);
            }
            // otherwise it must be unicode
            else{
                register_unicode(code);
            }


            return false;
        }
    }
    // finally check to see if we've activated a carousel
    if(record->event.pressed){
        for(int a = 0; a < CAROUSEL_MAX; a++){
            if(carousels[a].keycode == keycode){
                carousel = a;
                carousel_index = 0;
                return true;
            } 
        }
        carousel = -1;
    }
    return true;
}