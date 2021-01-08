#include "Game.hpp"
#include "utils.hpp"
#include <string.h>


static const char *colors[7] = {BLACK, RED, GREEN, YELLOW, BLUE, MAGENTA, CYAN};
/*--------------------------------------------------------------------------------

--------------------------------------------------------------------------------*/

static int str_to_num(string s){
    for (int i = 0; i < 7; ++i) {
        if (strcmp(s.c_str(), colors[i]) == 0){
            return i;
        }
    }
}

static string num_to_str(int num){
    return (string)colors[num];
}

/*--------------------------------------------------------------------------------
                            Perform phase functions
--------------------------------------------------------------------------------*/

static bool inbound(int i, int j, int lim_x, int lim_y){
    if ((i >= 0 && i < lim_x) && (j >= 0 && j < lim_y)){
        return true;
    }
    else{
        return false;
    }
}

static int dominant_species(int species_hist[]){
    int max_index=0;
    int max_appearance=0;
    for (int i = 1; i < 8; ++i) { //skip appearances of 0
        if (species_hist[i] > max_appearance){
            max_appearance = species_hist[i];
            max_index = i;
        }
    }
    return max_index;
}

static int color_in_next(Board& board, int i, int j, int row_len, int col_len, bool dead){
    int neighbour_count = 0;
    int species_hist[8] = {0,0,0,0,0,0,0,0};
    for (int k = -1; k <= 1 ; ++k) {
        for (int l = -1; l <= 1; ++l) {
            if (k == 0 && l == 0){
                continue;
            }
            if (inbound(i+k, j+l, row_len, col_len) && (board[i+k][j+l] > 0)){
                neighbour_count++;
                species_hist[board[i+k][j+l]]++;
            }
        }
    }
    if ( !dead && (neighbour_count == 3 || neighbour_count == 2)){
        return board[i][j];
    } else if ( dead && neighbour_count == 3){
        return dominant_species(species_hist);
    } else{
        return 0;
    }
}

static int conformism(Board& board, int i, int j, int row_len, int col_len){
    int neighbour_count = 0;
    int neighbour_sum = 0;
    for (int k = -1; k <= 1 ; ++k) {
        for (int l = -1; l <= 1; ++l) {
            if (inbound(i+k, j+l, row_len, col_len) && (board[i+k][j+l] > 0)){
                neighbour_count++;
                neighbour_sum += board[i+k][j+l];
            }
        }
    }
    return round(neighbour_sum/neighbour_count);
}


void Game::Preform_Phase(bool first_phase, int upper, int lower){
    int row_len = this->width;
    int col_len = this->height;
    for (int i = lower; i <= upper; ++i) {
        for (int j = 0; j < col_len; ++j) {
            bool dead = ((*this->board_curr)[i][j] == 0);
            if (first_phase){
                (*this->board_next)[i][j] = color_in_next(*this->board_curr, i, j, row_len, col_len, dead);
            } else if (!dead){ //we are in 2nd phase and the cell is alive, need to average it
                (*this->board_next)[i][j] = conformism(*this->board_curr, i, j, row_len, col_len);
            } else{
                (*this->board_next)[i][j] = 0;
            }
        }
    }
}

/*--------------------------------------------------------------------------------
                            Main game flow functions
--------------------------------------------------------------------------------*/

void Game::run() {

	_init_game(); // Starts the threads and all other variables you need
	print_board("Initial Board");
	for (uint i = 0; i < m_gen_num; ++i) {
		auto gen_start = std::chrono::system_clock::now();
		_step(i); // Iterates a single generation 
		auto gen_end = std::chrono::system_clock::now();
		m_gen_hist.push_back((double)std::chrono::duration_cast<std::chrono::microseconds>(gen_end - gen_start).count());
		print_board(nullptr);
	} // generation loop
	print_board("Final Board");
	_destroy_game();
}

void Game::_init_game() {
	// Create game fields - Consider using utils:read_file, utils::split
    auto string_board = new vector<vector<string>>;
    vector<string> lines = utils::read_lines(this->filename);
    for(auto & line : lines)
    {
        string_board->push_back(utils::split(line, ' '));
    }

    this->width = (*string_board)[0].size();
    this->height = string_board->size();
    this->board_curr = new Board;
    this->board_next = new Board;
    for(int i = 0; i < string_board->size(); i++)
    {
        vector<int> row;
        for(int j = 0; j < string_board[0].size(); j++)
        {
            row.push_back(std::stoi((*string_board)[i][j]));
        }
        this->board_curr->push_back(row);
        this->board_next->push_back(row);
    }
    delete string_board;
    this->rows_for_job = this->height / this->m_thread_num;
	// Create & Start threads

	for (uint i = 0; i < m_thread_num; ++i) {
        m_threadpool[i] = new GOLThread(i, *this);
        m_threadpool[i]->start();
    }
	// Testing of your implementation will presume all threads are started here

}

void Game::_step(uint curr_gen) {

    // Push jobs to queue (Phase 1)
    semph = Semaphore(-this->m_thread_num + 1);
    PushJobs(true);

	// Wait for the workers to finish calculating
	semph.down();

	// Swap pointers between current and next field
    SwapBoards(this->board_curr, this->board_next);

    // Push jobs to queue (Phase 2)
    semph = Semaphore(-this->m_thread_num + 1);
    PushJobs(false);

    // Wait for the workers to finish calculating
    semph.down();

    // Swap pointers between current and next field
    SwapBoards(this->board_curr, this->board_next);

    // NOTE: Threads must not be started here - doing so will lead to a heavy penalty in your grade
}

void Game::_destroy_game(){
	// Destroys board and frees all threads and resources
    delete this->board_next;
    delete this->board_curr;
    for (int i = 0; i < m_thread_num; ++i) {
        m_threadpool[i]->join();
    }

	// Not implemented in the Game's destructor for testing purposes. 
	// All threads must be joined here
}

/*--------------------------------------------------------------------------------
                            Our addition
--------------------------------------------------------------------------------*/

Game::Game(const game_params& gp) : interactive_on(gp.interactive_on), print_on(gp.print_on),
                             m_gen_num(gp.n_gen), m_thread_num(gp.n_thread), filename(gp.filename) { }

Game::~Game() { }

void Game::PushJobs(bool is_phase_one)
{
    job j;
    j.is_first_phase = is_phase_one;
    for(int i = 0; i < this->m_thread_num - 1; i++)
    {
        j.lower_row = i * this->rows_for_job;
        j.upper_row = ((i + 1) * this->rows_for_job) - 1;
        this->job_queue.push(j);
    }
    j.lower_row = (this->m_thread_num - 1) * this->rows_for_job;
    j.upper_row = this->height - 1;
    this->job_queue.push(j);
}

void Game::SwapBoards(Board* board1_p, Board* board2_p)
{
    Board* tmp_board_p = board1_p;
    board1_p = board2_p;
    board2_p = tmp_board_p;
}

/*--------------------------------------------------------------------------------
								
--------------------------------------------------------------------------------*/
inline void Game::print_board(const char* header) {

	if(print_on){ 

		// Clear the screen, to create a running animation 
		if(interactive_on)
			system("clear");

		// Print small header if needed
		if (header != nullptr)
			cout << "<------------" << header << "------------>" << endl;
		
		// TODO: Print the board
        cout << u8"╔" << string(u8"═") * this->width << u8"╗" << endl;
        for (uint i = 0; i < this->height; ++i) {
            cout << u8"║";
            for (uint j = 0; j < this->width; ++j) {
                if ((*this->board_curr)[i][j] > 0)
                    cout << colors[(*this->board_curr)[i][j] % 7] << u8"█" << RESET;
                else
                    cout << u8"░";
            }
            cout << u8"║" << endl;
        }
        cout << u8"╚" << string(u8"═") * this->width << u8"╝" << endl;

		// Display for GEN_SLEEP_USEC micro-seconds on screen 
		if(interactive_on)
			usleep(GEN_SLEEP_USEC);
	}

}


/* Function sketch to use for printing the board. You will need to decide its placement and how exactly 
	to bring in the field's parameters. 

		cout << u8"╔" << string(u8"═") * field_width << u8"╗" << endl;
		for (uint i = 0; i < field_height ++i) {
			cout << u8"║";
			for (uint j = 0; j < field_width; ++j) {
                if (field[i][j] > 0)
                    cout << colors[field[i][j] % 7] << u8"█" << RESET;
                else
                    cout << u8"░";
			}
			cout << u8"║" << endl;
		}
		cout << u8"╚" << string(u8"═") * field_width << u8"╝" << endl;
*/ 



