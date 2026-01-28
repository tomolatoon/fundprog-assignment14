#include "k2511070/component.h"

int main(){
    double time = 1.0;
    HLayer layer = k2511070_bound_one_note_layer(time);
    if(layer != NULL){
        layer_save_p3(layer,"test_output_layer1.ppm");
        layer_destroy(layer);
    }
    return 0;
}