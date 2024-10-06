# WordChecker 📝

**WordChecker** is a command-line tool designed to check the correspondence between characters in two words of equal length. The words can contain a variety of symbols including lowercase and uppercase alphabets, digits, hyphens, and underscores. This project was built as part of an algorithm and data structures module to practically apply the techniques learned.

## How the Game Works 🎮

- **Goal:** The objective is to guess the reference word by comparing it with other words.
- **Rules:**
  - The words are of equal length and contain letters, digits, hyphens (`-`), or underscores (`_`).
  - After each comparison, you receive feedback on whether the characters match:
    - `+`: Character is correct and in the right position.
    - `/`: Character is not present in the reference word.
    - `|`: Character is present in the word, but in the wrong position.
  - The player wins the match by guessing the correct word (`ok`), or the game ends if they fail after a set number of attempts (`ko`).
  - You can dynamically add or filter words that match the learned constraints during the game using commands like `+stampa_filtrate` (print_filtered), `+inserisci_inizio` (add_at_the_beginning), and `+inserisci_fine` (add_at_the_end).

## Features 🌟

- **Character Matching System:** Compares characters between two words and provides detailed feedback on whether they are in the correct position, present but misplaced, or absent.
- **Binary Search Tree (BST):** The program uses a BST to efficiently store and retrieve words based on lexicographical order.
- **Dynamic Word Management:** The system can dynamically add or remove words to/from the set of acceptable words during gameplay.
- **Game Mode:** Multiple "matches" can be initiated where the player tries to guess a word based on comparisons, with game controls like `+nuova_partita` (new_game), `+stampa_filtrate` (print_filtered), `+inserisci_inizio` (add_at_the_beginning), and `+inserisci_fine` (add_at_the_end).

## Testing 🧪

- The university testing system includes automated tests using test cases of varying complexity to ensure both correctness and efficiency.
- A grade is assigned based on how efficiently you perform the tasks provided, both in terms of time and space complexity.

## Built With 🛠️

- **Language:** C 
- **No external libraries:** This project is implemented using only the C standard library.

## Educational Purpose 🎓

This project was developed as part of the **Bachelor of Science in Computer Science Engineering** at **Politecnico di Milano** in Milan (MI), Italy. All data structures and functions/algorithms were implemented by me, fulfilling the objective of the project.

## Contributing 🙌

Contributions are welcome! Please feel free to submit a pull request or open an issue.

## License 📜

See the [LICENSE](LICENSE) file for more details.
