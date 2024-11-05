#include <iostream>
#include <fstream>
#include <vector>
#include <random>
#include <string>
#include <cmath>  // sin �Լ� ����� ���� ���

// sin ������ ���� ����� �߰��Ͽ� �����͸� �����ϴ� �Լ�
std::vector<std::pair<float, float>> generateSinDataWithNoise(int numDataPoints, float amplitude, float frequency, float noiseProbability, float noiseAmplitude) {
    std::vector<std::pair<float, float>> data;

    // ���� ������ �� ���� ����
    std::default_random_engine generator;
    std::uniform_real_distribution<float> noiseDist(-noiseAmplitude, noiseAmplitude); // ������ ����
    std::uniform_real_distribution<float> probabilityDist(0.0, 1.0); // ������ Ȯ��

    // sin ���� ������ ���� ����
    for (int i = 0; i < numDataPoints; ++i) {
        float x = i * frequency; // �ð� ���� ���� x �� ��� (���ļ��� �ݿ��Ͽ� �ֱ������� ��ȭ)

        // roll�� pitch�� ���� sin ���� ����
        float roll = amplitude * std::sin(x);
        float pitch = amplitude * std::sin(x);

        // ������ Ȯ���� ���� ������ �߰�
        if (probabilityDist(generator) < noiseProbability) {
            roll += noiseDist(generator);
            pitch += noiseDist(generator);
        }

        data.emplace_back(roll, pitch);
    }

    return data;
}

int main() {
    // sin �����Ϳ� ������ ����
    int numDataPoints = 2500;       // ������ ����Ʈ ��
    float amplitude = 30.0;         // sin ������ ����
    float frequency = 0.01;         // sin ������ ���ļ� (���� �������� ������ ��ȭ)
    float noiseProbability = 0.8;   // ������ �߻� Ȯ�� (80%)
    float noiseAmplitude = 1.0;     // �������� �ִ� ũ��

    // ������ ����
    auto sensorData = generateSinDataWithNoise(numDataPoints, amplitude, frequency, noiseProbability, noiseAmplitude);

    // CSV ���Ͽ� ������ ����
    std::ofstream outputFile("C:\\Users\\Minju\\source\\repos\\filer_test\\filer_test\\sin_data_with_noise.csv");
    outputFile << "Roll,Pitch\n";

    // ������ �����͸� ���Ͽ� ����
    for (const std::pair<float, float>& dataPair : sensorData) {
        float roll = dataPair.first;
        float pitch = dataPair.second;
        outputFile << roll << "," << pitch << "\n";
    }

    outputFile.close();
    std::cout << "Sin wave data with noise saved to sin_data_with_noise.csv" << std::endl;

    return 0;
}
