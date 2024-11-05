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
        predict();               // Prediction �ܰ�
        return correct(measurement);  // Correction �ܰ�
    }

private:
    float Q; // Process noise
    float R; // Measurement noise
    float P; // Estimation error
    float X; // Value (angle)
};

// CSV ���Ͽ��� �����͸� �о���� �Լ�
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
    // Į�� ���� �ʱ�ȭ
    KalmanFilter kalmanRoll(0.01, 0.1, 1, 0);
    KalmanFilter kalmanPitch(0.01, 0.1, 1, 0);

    // CSV ���Ͽ��� �����͸� �о��
    std::vector<std::vector<float>> sensorData = readCSV("C:\\Users\\Minju\\source\\repos\\filer_test\\filer_test\\constant_data_with_noise.csv");

    if (sensorData.empty()) {
        std::cerr << "No data read from file." << std::endl;
        return 1;
    }

    // ���͸��� �����͸� ������ ���� ����
    std::ofstream outputFile("C:\\Users\\Minju\\source\\repos\\filer_test\\filer_test\\filtered_data3.csv");
    outputFile << "OriginalRoll,OriginalPitch,PredictedRoll,CorrectedRoll,FilteredRoll,PredictedPitch,CorrectedPitch,FilteredPitch\n";

    // �� ���� �����͸� Į�� ���Ϳ� ����
    for (const auto& row : sensorData) {
        if (row.size() < 2) {
            std::cerr << "Row has insufficient data." << std::endl;
            continue;
        }

        float roll = row[0];
        float pitch = row[1];

        // Roll ������ Prediction �� Correction �ܰ� ����
        float predictedRoll = kalmanRoll.predict();
        float correctedRoll = kalmanRoll.correct(roll);
        float filteredRoll = kalmanRoll.update(roll); // Prediction + Correction

        // Pitch ������ Prediction �� Correction �ܰ� ����
        float predictedPitch = kalmanPitch.predict();
        float correctedPitch = kalmanPitch.correct(pitch);
        float filteredPitch = kalmanPitch.update(pitch); // Prediction + Correction

        // ����� ���Ͽ� ����
        outputFile << roll << "," << pitch << ","
            << predictedRoll << "," << correctedRoll << "," << filteredRoll << ","
            << predictedPitch << "," << correctedPitch << "," << filteredPitch << "\n";
    }

    outputFile.close();
    std::cout << "Filtered data with prediction and correction saved to filtered_data.csv" << std::endl;

    return 0;
}
