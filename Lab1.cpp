#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>

using namespace std;
mutex mtx;  // Мьютекс для синхронизации
condition_variable cv;  // Условная переменная для ожидания события
bool provider_active = true;

// Функция для потока-поставщика
void provider_thread() {
    while (true) {
        this_thread::sleep_for(chrono::seconds(1));  // Задержка в 1 секунду
        unique_lock<mutex> lock(mtx);
        cout << "Поставщик: отправляю событие" << endl;
        provider_active = false;
        cv.notify_one();  // Сообщаем потребителю, что событие отправлено
    }
}

// Функция для потока-потребителя
void consumer_thread() {
    while (true) {
        unique_lock<mutex> lock(mtx);
        cv.wait(lock, [] { return !provider_active; });  // Ожидаем, если активен поставщик
        cout << "Потребитель: получил событие" << endl;
	cout << endl;
        provider_active = true;
        cv.notify_one();  // Сообщаем поставщику, что событие обработано
    }
}

int main() {
    thread provider(provider_thread);
    thread consumer(consumer_thread);
    provider.join();
    consumer.join();
    return 0;
}