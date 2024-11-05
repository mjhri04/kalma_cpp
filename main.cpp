#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>

// ������ Į�� ���� Ŭ����
class KalmanFilter {
public:
    KalmanFilter(float processNoise, float measurementNoise, float estimateError, float initialEstimate) {
        Q = processNoise;
        R = measurementNoise;
        P = estimateError;
        X = initialEstimate;
    }

    // ���͸��� ���� ������Ʈ�ϴ� �Լ�
    float update(float measurement) {
        // ���� �ܰ�
        P = P + Q;

        // ���� �ܰ�
        float K = P / (P + R);    // Į�� �̵�
        X = X + K * (measurement - X);  // ������ ������Ʈ
        P = (1 - K) * P;               // ���� ���� ������Ʈ

        return X;
    }

private:
    float Q; // ���μ��� ������
    float R; // ���� ������
    float P; // ���� ����
    float X; // ���͸��� �� (������)
};

// CSV ���Ͽ��� �����͸� �о���� �Լ�
std::vector<std::pair<float, float>> readCSV(const std::string& filename) {
    std::vector<std::pair<float, float>> data;
    std::ifstream file(filename);
    std::string line;

    if (!file.is_open()) {
        std::cerr << "Error opening file: " << filename << std::endl;
        return data;
    }

    while (std::getline(file, line)) {
        std::stringstream ss(line);
        std::string rollStr, pitchStr;

        // �� �࿡�� Roll, Pitch ���� �о����
        if (std::getline(ss, rollStr, ',') && std::getline(ss, pitchStr, ',')) {
            try {
                float roll = std::stof(rollStr);
                float pitch = std::stof(pitchStr);
                data.emplace_back(roll, pitch);
            }
            catch (const std::invalid_argument& e) {
                std::cerr << "Invalid data in CSV file: " << line << std::endl;
            }
        }
    }

    file.close();
    return data;
}

int main() {
    // �Է� �����͸� ������ CSV ���� ���
    std::string inputFileName = "C:\\4imu_data.csv";
    std::string outputFileName = "C:\\Users\\Minju\\source\\repos\\filer_test\\filer_test\\filtered_data4.csv";

    // �Է� CSV ���Ͽ��� �����͸� �о��
    std::vector<std::pair<float, float>> originalData = readCSV(inputFileName);
    if (originalData.empty()) {
        std::cerr << "No data read from file." << std::endl;
        return 1;
    }

    // Į�� ���� �ʱ�ȭ
    KalmanFilter kalmanRoll(0.01, 0.1, 1, 0);   // Roll�� ���� Į�� ���� �ʱ�ȭ
    KalmanFilter kalmanPitch(0.01, 0.1, 1, 0);  // Pitch�� ���� Į�� ���� �ʱ�ȭ

    // ���͸��� �����͸� ������ CSV ���� ����
    std::ofstream outputFile(outputFileName);
    outputFile << "OriginalRoll,OriginalPitch,FilteredRoll,FilteredPitch\n";  // CSV ��� �߰�

    // ���� �����͸� ���Ϳ� ������Ѽ� ���͸��� �����͸� �����ϰ� ���Ͽ� ����
    for (const auto& dataPair : originalData) {
        float roll = dataPair.first;
        float pitch = dataPair.second;

        // Į�� ���͸� ���� ������ ���͸�
        float filteredRoll = kalmanRoll.update(roll);
        float filteredPitch = kalmanPitch.update(pitch);

        // ���� �����Ϳ� ���͸��� �����͸� CSV ���Ͽ� ����
        outputFile << roll << "," << pitch << "," << filteredRoll << "," << filteredPitch << "\n";
    }

    outputFile.close();  // ���� �ݱ�
    std::cout << "Filtered data saved to " << outputFileName << std::endl;

    return 0;
}
