#include <iostream>
#include <cstdint>
#include <fstream>
#include <cmath>

#define XCORR_EPSILON 0.1
#define BOTH_SPECTRUMS 2

using namespace std;

using float32_t = float;


//***************************************************************
//  Perform the cross correlation of the target signal and the
//	sampled signal. Return which side of the spectrum the peak
//  value was found. This is to save memory by avoiding having
//  to store all the calculations.
//
uint8_t spectrum_peak(
    float32_t* pSrcA,
    float32_t* pSrcB,
    uint32_t blockSize,
    float32_t* dest = NULL
    );

float32_t xcorr_left_max(
    float32_t* pSrcA,
    float32_t* pSrcB,
    uint32_t blockSize,
    float32_t* dest = NULL
    );

float32_t xcorr_right_max(
    float32_t* pSrcA,
    float32_t* pSrcB,
    uint32_t blockSize,
    float32_t* dest = NULL
    );

int main() {
	// your code goes here
	float32_t i_arr[100] = {0.0f};
	float32_t q_arr[100] = {0.0f};
	float32_t q_arr2[100] = {0.0f};
    float32_t result1[200] = {0.0f}, result2[200] = {0.0f};

    for(unsigned i = 0, val_counter = 0, q_val1 = 0, q_val2 = 0; i < 100; ++i){
        if(i >= 40 && i <= 60){
            i_arr[i] = val_counter++;
        }
        if(i >= 5 && i <= 25){
            q_arr[i] = q_val1++;
        }
        if(i >= 75 && i <= 95){
            q_arr2[i] = q_val2++;
        }
    }

    xcorr_left_max(i_arr, q_arr, 100, result1);
    xcorr_right_max(i_arr, q_arr, 100, result1+100);

    xcorr_left_max(i_arr, q_arr2, 100, result2);
    xcorr_right_max(i_arr, q_arr2, 100, result2+100);

    std::ofstream csv_file("xcorr_out.csv");
    if(!csv_file.is_open()){
        std::cerr << "Error. Could not open file ./xcorr_out.csv.\n";
        return 0;
    }
    csv_file << "index,i_arr,q_arr,q_arr2,result1,result2\n";

    for(unsigned i = 0; i < 200; ++i){
        csv_file << i << ',';
        if(i < 100){
            csv_file << i_arr[i] << ',' << q_arr[i] << ',' << q_arr2[i];
        } else {
            csv_file << ",,";
        }
        csv_file << ',' << result1[i] << ',' << result2[i] << '\n';
    }

    csv_file << std::flush;
    csv_file.close();
	
	return 0;
}


//***************************************************************
//  Perform the cross correlation of the target signal and the
//	sampled signal. Return which side of the spectrum the peak
//  value was found. This is to save memory by avoiding having
//  to store all the calculations.
//
uint8_t spectrum_peak(
    float32_t* pSrcA,
    float32_t* pSrcB,
    uint32_t blockSize
    )
{
    float32_t
            left_peak = xcorr_left_max(pSrcA, pSrcB, blockSize),
            right_peak = xcorr_right_max(pSrcA, pSrcB, blockSize)
            ;
    if(fabs(left_peak - right_peak) < XCORR_EPSILON)
    {
        return BOTH_SPECTRUMS;
    }
    return left_peak > right_peak;
}

float32_t xcorr_left_max(
    float32_t* pSrcA,
    float32_t* pSrcB,
    uint32_t blockSize,
    float32_t* dest
    )
{
    uint32_t i = 0, j = 0;
    float32_t toreturn = 0.0;
    float32_t calc_hold = 0.0;

        // Slide srcA left-to-right starting from no overlap
    for(i=0; i < blockSize; ++i){
        calc_hold = 0;
        for(j=0; j+i < blockSize; ++j){
            calc_hold += pSrcA[j+i] * pSrcB[j];
        }
        if(i == 0 || toreturn < calc_hold)
        {
            toreturn = calc_hold;
        }
        dest[i] = calc_hold;
    }
    return toreturn;
}

float32_t xcorr_right_max(
    float32_t* pSrcA,
    float32_t* pSrcB,
    uint32_t blockSize,
    float32_t* dest
    )
{
    uint32_t i = 0, j = 0;
    float32_t toreturn = 0.0;
    float32_t calc_hold = 0.0;

        // Slide srcA left-to-right starting from complete overlap
    for(i=0; i < blockSize; ++i){
        calc_hold = 0;
        for(j=0; j+i < blockSize; ++j){
            calc_hold += pSrcA[j] * pSrcB[j + i];
        }
        if(i == 0 || toreturn < calc_hold)
        {
            toreturn = calc_hold;
        }
        dest[i] = calc_hold;
    }
    return toreturn;
}
