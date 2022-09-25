#include <iostream>
#include <algorithm>

#include <ncurses.h>
#include <vector>
#include <cmath>
#include <fstream>
#include <string>
#include <cstring>
#include <unistd.h>

enum TodoStatus{
  DONE, TODO
};
int row = 0, column = 0;


void loadTodoListsFromFile(const char *file,
    std::vector<std::string> &todos,
    std::vector<TodoStatus> &status)
{
  (void) todos;
  (void) status;
  std::ifstream t_file(file);
  char line[256];
  while(t_file.getline(line, 256))
  {
    std::string line_s = std::string(line);
    todos.push_back(line_s.substr(line_s.find(":")).c_str() + 1);
    status.push_back(line_s.substr(0, line_s.find(":")) == "DONE" ? DONE : TODO);
  }
  for(int i = 0; i < 12; i++)
  {
    std::string txt = std::to_string(i * 100 / 12) + "/100";

    mvprintw(row / 2, column / 2 - std::floor(txt.size() / 2), txt.c_str());
    refresh();
    usleep(250000);
    clear();
  }
}

void saveTodosVectors(const char *file,
    std::vector<std::string> &todos,
    std::vector<TodoStatus> &status)
{
  std::ofstream t_file(file);

  for(std::size_t i = 0; i < todos.size(); i++)
  {
    std::string s(status[i] == DONE ? "DONE" : "TODO"), result = s + ":" + todos[i] + "\n";
    t_file.write(result.c_str(), result.size());
  }
}

int main(int argc, const char *argv[])
{
  if(argc < 2)
  {
    std::cout << argv[0] << " [file name]\n";
    return EXIT_FAILURE;
  }
  initscr();
  raw();
  noecho();
  curs_set(0);

  std::vector<std::string> todos;
  std::vector<TodoStatus> todos_status;


  std::size_t current_todo = 0;
  char key;
  bool quit = false;

  getmaxyx(stdscr, row, column);
  loadTodoListsFromFile(argv[1], todos, todos_status);
  while(!quit)
  {
    for(std::size_t index = 0; index < todos.size(); index++)
    {
      TodoStatus status = todos_status[index];
      char option[256];
      strcpy(option, status == DONE ? "[X] " : "[ ] ");
      
      mvprintw(0, column / 2 - 5, "To-Do List");
      if(current_todo == index)
        attron(A_BOLD);
      mvprintw(index + 2, 4, strcat(option, todos[index].c_str()));
      attroff(A_BOLD);
    }
    key = getch();
    switch(key)
    {
      case 10:
        todos_status[current_todo] = DONE;
        break;
      case 'd':
        todos_status[current_todo] = TODO;
        break;
      case 66:
        current_todo += current_todo >= todos.size() ? 0 : 1;
        break;
      case 65:
        current_todo -= current_todo <= 0 ? 0 : 1;
        break;
      case 'q':
        quit = true;
      default:
        break;
    }
    

    refresh();
    mvprintw(row - 1, 1, ("Laset key pressed: " + std::to_string(key)).c_str());
  }
  
  endwin();
  saveTodosVectors(argv[1], todos, todos_status);
  curs_set(1);
  return EXIT_SUCCESS;
}
