import subprocess
import os
import pytest

BUILD_SCRIPT = "./build.sh"
PARSER = "./parser"
TESTS_FOLDER = "tests"


@pytest.fixture(scope="session", autouse=True)
def compile_parser():
    result = subprocess.run(["bash", BUILD_SCRIPT])
    assert result.returncode == 0, \
        "Failed to compile './parser'. Build script output:\n"\
        f"{result.stdout}\n{result.stderr}"


@pytest.mark.parametrize("test_name", [
    "comment",
    "general",
    "lexer_error",
    "minio",
    "parser_error"
])
def test_parser(test_name):
    assert os.path.isfile(PARSER), f"Program '{PARSER}' not found."

    file_path_in = f"{TESTS_FOLDER}/test_{test_name}.in"
    file_path_res = f"{TESTS_FOLDER}/test_{test_name}.res"

    process = subprocess.run([PARSER, file_path_in],
                             capture_output=True, text=True)

    assert process.returncode == 0, \
        f"Program exited with code {process.returncode}"

    with open(file_path_res, "r") as ans_file:
        expected_output = ans_file.read()

    assert process.stdout == expected_output, (
        "Output does not match expected result.\n\n"
        f"=== Actual Output ===\n{process.stdout}\n\n"
        f"=== Expected Output ===\n{expected_output}"
    )
