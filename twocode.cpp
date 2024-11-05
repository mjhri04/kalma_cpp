#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>

class KalmanFilter {
public:
    KalmanFilter(float processNoise, float measurementNoise, float estimateError, float initialEstimate) {
        Q = processNoise;
        R = measurementNoise;
        P = estimateError;
        X = initialEstimate;
    }

    float predict() {
        P = P + Q; // Prediction Update
        return X;
    }

    float correct(float measurement) {
        float K = P / (P + R);
        X = X + K * (measurement - X);
        P = (1 - K) * P;
        return X;
    }

    float update(float measurement) {
        // Combined Prediction and Correction step
        predict();               // Prediction 단계
        return correct(measurement);  // Correction 단계
    }

private:
    float Q; // Process noise
    float R; // Measurement noise
    float P; // Estimation error
    float X; // Value (angle)
};

// CSV 파일에서 데이터를 읽어오는 함수
std::vector<std::vector<float>> readCSV(const std::string& filename) {
    std::vector<std::vector<float>> data;
    std::ifstream file(filename);
    std::string line;

    if (!file.is_open()) {
        std::cerr << "Error opening file: " << filename << std::endl;
        return data;
    }

    while (std::getline(file, line)) {
        std::vector<float> row;
        std::stringstream ss(line);
        std::string value;

        while (std::getline(ss, value, ',')) {
            try {
                row.push_back(std::stof(value));
            }
            catch (...) {
                row.push_back(0.0f);
            }
        }
        if (!row.empty()) {
            data.push_back(row);
        }
    }
    return data;
}

int main() {
    // 칼만 필터 초기화
    KalmanFilter kalmanRoll(0.01, 0.1, 1, 0);
    KalmanFilter kalmanPitch(0.01, 0.1, 1, 0);

    // CSV 파일에서 데이터를 읽어옴
    std::vector<std::vector<float>> sensorData = readCSV("C:\\Users\\Minju\\source\\repos\\filer_test\\filer_test\\constant_data_with_noise.csv");

    if (sensorData.empty()) {
        std::cerr << "No data read from file." << std::endl;
        return 1;
    }

    // 필터링된 데이터를 저장할 파일 열기
    std::ofstream outputFile("C:\\Users\\Minju\\source\\repos\\filer_test\\filer_test\\filtered_data3.csv");
    outputFile << "OriginalRoll,OriginalPitch,PredictedRoll,CorrectedRoll,FilteredRoll,PredictedPitch,CorrectedPitch,FilteredPitch\n";

    // 각 행의 데이터를 칼만 필터에 적용
    for (const auto& row : sensorData) {
        if (row.size() < 2) {
            std::cerr << "Row has insufficient data." << std::endl;
            continue;
        }

        float roll = row[0];
        float pitch = row[1];

        // Roll 필터의 Prediction 및 Correction 단계 수행
        float predictedRoll = kalmanRoll.predict();
        float correctedRoll = kalmanRoll.correct(roll);
        float filteredRoll = kalmanRoll.update(roll); // Prediction + Correction

        // Pitch 필터의 Prediction 및 Correction 단계 수행
        float predictedPitch = kalmanPitch.predict();
        float correctedPitch = kalmanPitch.correct(pitch);
        float filteredPitch = kalmanPitch.update(pitch); // Prediction + Correction

        // 결과를 파일에 저장
        outputFile << roll << "," << pitch << ","
            << predictedRoll << "," << correctedRoll << "," << filteredRoll << ","
            << predictedPitch << "," << correctedPitch << "," << filteredPitch << "\n";
    }

    outputFile.close();
    std::cout << "Filtered data with prediction and correction saved to filtered_data.csv" << std::endl;

    return 0;
}
