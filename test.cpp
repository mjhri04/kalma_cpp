#include <iostream>
#include <fstream>
#include <vector>
#include <random>
#include <string>
#include <cmath>  // sin 함수 사용을 위한 헤더

// sin 파형에 랜덤 노이즈를 추가하여 데이터를 생성하는 함수
std::vector<std::pair<float, float>> generateSinDataWithNoise(int numDataPoints, float amplitude, float frequency, float noiseProbability, float noiseAmplitude) {
    std::vector<std::pair<float, float>> data;

    // 난수 생성기 및 분포 설정
    std::default_random_engine generator;
    std::uniform_real_distribution<float> noiseDist(-noiseAmplitude, noiseAmplitude); // 노이즈 범위
    std::uniform_real_distribution<float> probabilityDist(0.0, 1.0); // 노이즈 확률

    // sin 파형 데이터 생성 루프
    for (int i = 0; i < numDataPoints; ++i) {
        float x = i * frequency; // 시간 축을 위한 x 값 계산 (주파수를 반영하여 주기적으로 변화)

        // roll과 pitch에 각각 sin 값을 설정
        float roll = amplitude * std::sin(x);
        float pitch = amplitude * std::sin(x);

        // 지정된 확률에 따라 노이즈 추가
        if (probabilityDist(generator) < noiseProbability) {
            roll += noiseDist(generator);
            pitch += noiseDist(generator);
        }

        data.emplace_back(roll, pitch);
    }

    return data;
}

int main() {
    // sin 데이터와 노이즈 설정
    int numDataPoints = 2500;       // 데이터 포인트 수
    float amplitude = 30.0;         // sin 파형의 진폭
    float frequency = 0.01;         // sin 파형의 주파수 (값이 작을수록 느리게 변화)
    float noiseProbability = 0.8;   // 노이즈 발생 확률 (80%)
    float noiseAmplitude = 1.0;     // 노이즈의 최대 크기

    // 데이터 생성
    auto sensorData = generateSinDataWithNoise(numDataPoints, amplitude, frequency, noiseProbability, noiseAmplitude);

    // CSV 파일에 데이터 저장
    std::ofstream outputFile("C:\\Users\\Minju\\source\\repos\\filer_test\\filer_test\\sin_data_with_noise.csv");
    outputFile << "Roll,Pitch\n";

    // 생성한 데이터를 파일에 저장
    for (const std::pair<float, float>& dataPair : sensorData) {
        float roll = dataPair.first;
        float pitch = dataPair.second;
        outputFile << roll << "," << pitch << "\n";
    }

    outputFile.close();
    std::cout << "Sin wave data with noise saved to sin_data_with_noise.csv" << std::endl;

    return 0;
}
