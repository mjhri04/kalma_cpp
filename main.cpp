#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>

// 간단한 칼만 필터 클래스
class KalmanFilter {
public:
    KalmanFilter(float processNoise, float measurementNoise, float estimateError, float initialEstimate) {
        Q = processNoise;
        R = measurementNoise;
        P = estimateError;
        X = initialEstimate;
    }

    // 필터링된 값을 업데이트하는 함수
    float update(float measurement) {
        // 예측 단계
        P = P + Q;

        // 수정 단계
        float K = P / (P + R);    // 칼만 이득
        X = X + K * (measurement - X);  // 추정값 업데이트
        P = (1 - K) * P;               // 추정 오차 업데이트

        return X;
    }

private:
    float Q; // 프로세스 노이즈
    float R; // 측정 노이즈
    float P; // 추정 오차
    float X; // 필터링된 값 (추정값)
};

// CSV 파일에서 데이터를 읽어오는 함수
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

        // 각 행에서 Roll, Pitch 값을 읽어들임
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
    // 입력 데이터를 저장한 CSV 파일 경로
    std::string inputFileName = "C:\\4imu_data.csv";
    std::string outputFileName = "C:\\Users\\Minju\\source\\repos\\filer_test\\filer_test\\filtered_data4.csv";

    // 입력 CSV 파일에서 데이터를 읽어옴
    std::vector<std::pair<float, float>> originalData = readCSV(inputFileName);
    if (originalData.empty()) {
        std::cerr << "No data read from file." << std::endl;
        return 1;
    }

    // 칼만 필터 초기화
    KalmanFilter kalmanRoll(0.01, 0.1, 1, 0);   // Roll에 대한 칼만 필터 초기화
    KalmanFilter kalmanPitch(0.01, 0.1, 1, 0);  // Pitch에 대한 칼만 필터 초기화

    // 필터링된 데이터를 저장할 CSV 파일 열기
    std::ofstream outputFile(outputFileName);
    outputFile << "OriginalRoll,OriginalPitch,FilteredRoll,FilteredPitch\n";  // CSV 헤더 추가

    // 원본 데이터를 필터에 통과시켜서 필터링된 데이터를 생성하고 파일에 저장
    for (const auto& dataPair : originalData) {
        float roll = dataPair.first;
        float pitch = dataPair.second;

        // 칼만 필터를 통해 데이터 필터링
        float filteredRoll = kalmanRoll.update(roll);
        float filteredPitch = kalmanPitch.update(pitch);

        // 원본 데이터와 필터링된 데이터를 CSV 파일에 저장
        outputFile << roll << "," << pitch << "," << filteredRoll << "," << filteredPitch << "\n";
    }

    outputFile.close();  // 파일 닫기
    std::cout << "Filtered data saved to " << outputFileName << std::endl;

    return 0;
}
