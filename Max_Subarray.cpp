#include <iostream>
#include <vector>
#define INT_MIN -9999
#define MAX_SIZE 100
using namespace std;

struct INDEX{
	int low;
	int high;
	int sum;
};

struct INDEX Find_Max_Crossing_Subarray(int* arr,struct INDEX c) {
	int mid = (c.high + c.low) / 2;
	int i = mid;     
	int j = mid + 1; 
	int left_sum = INT_MIN;
	int right_sum = INT_MIN;
	int sum = 0;
	int low = c.low;
	int high = c.high;
	for (; i > low ; i-- ) { //找出左半边数组的最大子数组，从mid开始
		sum = sum + arr[i];
		if (sum > left_sum) {
			c.low = i;
			left_sum = sum;
		}
	}
	sum = 0;
	for (; j < high; j++) { //同理，找出右半边
		sum = sum + arr[j];
		if (sum > right_sum) {
			c.high = j;
			right_sum = sum;
		}
	}
	c.sum = left_sum + right_sum;
	if (left_sum > c.sum)
		c.sum = left_sum;
	else if (right_sum > c.sum)
		c.sum = right_sum;
	return c;
}

struct INDEX Find_Max_Subarray(int* arr, struct INDEX c) {
	if (c.high == c.low) {
		c.sum = c.low;
		return c;
	}
	int mid = (c.high + c.low) / 2;
	struct INDEX left;
	left.high = mid;
	left.low = c.low;
	struct INDEX right;
	right.high = c.high;
	right.low = mid + 1;
	left = Find_Max_Subarray(arr, left);
	right = Find_Max_Subarray(arr, right);
	c = Find_Max_Crossing_Subarray(arr, c);
	if (left.sum >= right.sum && left.sum >= c.sum)
		return left;
	else if (right.sum >= left.sum && right.sum >= c.sum)
		return right;
	else return c;
}



int main() {
	int n = 0;
	int num = 0;
	int arr[MAX_SIZE];
	cin >> n;
	for (int i = 0; i < n; i++) {
		cin >> arr[i];
	}
	struct INDEX c;
	c.high = n;
	c.low = 0;
	c = Find_Max_Subarray(arr,c);
	std::cout << c.high << " " << c.low << " " << c.sum;
	return 0;
}
