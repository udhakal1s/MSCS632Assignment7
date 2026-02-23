# Umesh Dhakal
# Python- MSCS632A7
# Mean, Median and Mode of the integers we eneter

import sys


class StatisticsCalculator:
    # OOP class to calculate mean, median, mode

    def __init__(self, num_list):
        if not num_list:
            raise ValueError("List is empty.")
        self.num_list = num_list

    def get_mean(self):
        total_sum = 0
        for value in self.num_list:
            total_sum += value
        return total_sum / len(self.num_list)

    def get_median(self):
        sorted_nums = sorted(self.num_list)
        n = len(sorted_nums)
        mid_index = n // 2

        if n % 2 == 1:
            return float(sorted_nums[mid_index])
        else:
            left_val = sorted_nums[mid_index - 1]
            right_val = sorted_nums[mid_index]
            return (left_val + right_val) / 2.0

    def get_mode(self):
        # Mode
        freq_table = {}
        for value in self.num_list:
            if value in freq_table:
                freq_table[value] += 1
            else:
                freq_table[value] = 1

        max_freq = 0
        for value in freq_table:
            if freq_table[value] > max_freq:
                max_freq = freq_table[value]

        # If all appear once, no mode
        if max_freq == 1:
            return None

        mode_list = []
        for value in freq_table:
            if freq_table[value] == max_freq:
                mode_list.append(value)

        mode_list.sort()
        return mode_list


def parse_numbers_from_commas(text_line):
    parts = text_line.split(",")

    number_list = []
    for item in parts:
        cleaned = item.strip()
        if cleaned == "":
            continue
        number_list.append(int(cleaned))

    if len(number_list) == 0:
        raise ValueError("No numbers found.")

    return number_list


def read_input_numbers():
    if len(sys.argv) > 1:
        joined = " ".join(sys.argv[1:]).strip()
        if "," in joined:
            return parse_numbers_from_commas(joined)
        else:
            return [int(x) for x in sys.argv[1:]]
            
        #input integers
    user_line = input("Enter all the integers separated by commas: ").strip()
    return parse_numbers_from_commas(user_line)


def main():
    try:
        numbers_entered = read_input_numbers()

        stats_obj = StatisticsCalculator(numbers_entered)

        mean_result = stats_obj.get_mean()
        median_result = stats_obj.get_median()
        mode_result = stats_obj.get_mode()

        print(f"Total number of integer you have {len(numbers_entered)}")
        print(f"The mean of all the number you enter  is {mean_result:.4f}")
        print(f"The median of all the number you enter  is {median_result:.4f}")

        if mode_result is None:
            print("There is no mode")
        else:
            # if multiple modes, show as comma list
            mode_text = ", ".join(str(x) for x in mode_result)
            print(f"The mode of all the number you enter  is {mode_text}")

    except ValueError as e:
        print(f"Input error: {e}")
    except Exception as e:
        print(f"Error: {e}")


if __name__ == "__main__":
    main()
