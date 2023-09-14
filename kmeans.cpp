#include <iostream>
#include <vector>
#include <cmath>
#include <cstdlib>
#include <ctime>
using namespace std;

// Класс для представления точек
class Point {
public:
    double x;
    double y;

    Point(double x, double y) : x(x), y(y) {}

    double distance(const Point& other) const {
        double dx = x - other.x;
        double dy = y - other.y;
        return sqrt(dx * dx + dy * dy);
    }
};

// Класс для алгоритма k-средних
class KMeans {
private:
    int k;
    vector<Point> data;
    vector<Point> centroids;

public:
    KMeans(int k, const vector<Point>& data) : k(k), data(data) {
        // Инициализация центроидов случайными точками
        srand(static_cast<unsigned int>(time(nullptr)));
        for (int i = 0; i < k; ++i) {
            int index = rand() % data.size();
            centroids.push_back(data[index]);
        }
    }
    ~KMeans()
    {
        data.clear();
        centroids.clear();
    }

    void cluster() {
        bool converged = true;
        while (converged) {
            // Назначаем точки к ближайшим центроидам
            vector<int> clusterAssignments;
            for (const Point& point : data) {
                double minDistance = numeric_limits<double>::max();
                int closestCentroid = -1;

                for (int i = 0; i < k; ++i) {
                    double distance = point.distance(centroids[i]);
                    if (distance < minDistance) {
                        minDistance = distance;
                        closestCentroid = i;
                    }
                }

                clusterAssignments.push_back(closestCentroid);
            }

            // Пересчитываем центроиды
            vector<Point> newCentroids(k, Point(0.0, 0.0));
            vector<int> clusterSizes(k, 0);

            for (size_t i = 0; i < data.size(); ++i) {
                int cluster = clusterAssignments[i];
                newCentroids[cluster].x += data[i].x;
                newCentroids[cluster].y += data[i].y;
                clusterSizes[cluster]++;
            }

            for (int i = 0; i < k; ++i) {
                if (clusterSizes[i] > 0) {
                    newCentroids[i].x /= clusterSizes[i];
                    newCentroids[i].y /= clusterSizes[i];
                }
            }

            // Проверяем на сходимость
            for (int i = 0; i < k && converged; ++i) {
               if (newCentroids[i].x == centroids[i].x && newCentroids[i].y == centroids[i].y)
                   converged = false;
            }

            centroids = newCentroids;
            newCentroids.clear();
        }
    }

    void printClusters() {
        for (int i = 0; i < k; ++i) {
            cout << "Cluster " << i + 1 << " (centroid: " << centroids[i].x << ", " << centroids[i].y << "):" << endl;
            for (size_t j = 0; j < data.size(); ++j) {
                if (pointCluster(data[j]) == i) {
                    cout << "(" << data[j].x << ", " << data[j].y << ")" << endl;
                }
            }
            cout << endl;
        }
    }

    int pointCluster(const Point& point) const {
        double minDistance = numeric_limits<double>::max();
        int closestCentroid = -1;

        for (int i = 0; i < k; ++i) {
            double distance = point.distance(centroids[i]);
            if (distance < minDistance) {
                minDistance = distance;
                closestCentroid = i;
            }
        }

        return closestCentroid;
    }

};

int main() {
    // Пример данных
    vector<Point> data = { Point(1, 2), Point(2, 3), Point(3, 4), Point(5, 6), Point(6, 7), Point(7, 8) };

    // Создаем объект алгоритма k-средних
    int k = 2;
    KMeans kmeans(k, data);

    // Кластеризуем данные 
    kmeans.cluster();

    // Выводим результат кластеризации
    kmeans.printClusters();

    data.clear();

    
    return 0;
}