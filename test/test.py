import os
import sys
import subprocess


def compare_with_correct_answers(run_prog, test_dir, perfect = False):

    test_complited  = True
    run_file_error  = False
    right_answers = 0
    wrong_answers = []
    prog_errors = []

    for test_file in os.listdir(test_dir):
        correct_answer = int(test_file.split('_')[-2 + perfect])

        file_name = os.path.join(test_dir, test_file)
        num_of_test = int(test_file.split('_')[1])
        
        try:
            program_answer = subprocess.run(run_prog, stdin=open(file_name), stdout=subprocess.PIPE, text=True).stdout
            program_answer = int(program_answer.split(':')[-1])

            if correct_answer == program_answer:
                print("-------------------------------------------")
                print(f"Test {num_of_test} completed successfully\n")
                right_answers += 1
            else:
                print("--------------------------------------------")
                print(f"Test {num_of_test} has failed: answer: {program_answer}, correct: {correct_answer}\n")
                test_complited = False
                wrong_answers.append(num_of_test)
        except:
            print("-------------------------------------------")
            print(f"Test {num_of_test} completed with an error\n")
            run_file_error = True
            prog_errors.append(num_of_test)
            

    if run_file_error:
        print(f"{run_prog} completed with errors")
        print(f"Indexes of error tests: {prog_errors}")
    elif test_complited:
        print(f"{run_prog} complited all tests!")
    else:
        print(f"{run_prog} complited {right_answers} out of {len(os.listdir(test_dir))}")
        print(f"Indexes of wrong tests: {wrong_answers}")


if __name__ == "__main__":
    p_flag = False

    if len(sys.argv) == 4 and sys.argv[3] == '-p':
        p_flag = True
    elif len(sys.argv) != 3: 
        exit("Usage: python script.py RUN_FILE TEST_DIR")

    run_prog, test_dir = sys.argv[1], sys.argv[2]
    compare_with_correct_answers(run_prog, test_dir, p_flag)
    sys.exit(0)