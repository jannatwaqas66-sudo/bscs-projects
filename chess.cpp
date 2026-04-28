#include <iostream>
using namespace std;
int chess();
int main()
{
    cout << "PROJECT : CHESS\nMADE BY\nAQSA ISMAIL     L1F25BSCS0055\nJANNAT WAQAS    L1F25BSCS0019\nAmna Tariq      L1F25BSCS\n";
    system("pause");
    char choice;
    cout << "Enter your Choice\nType Y if you want to play\nType N if you want to exit\n";
    cin >> choice;
    if (choice == 'Y')
    {
        cout << "CHESS IS LOADING" << endl;
        system("pause");
        chess();
    }
    if (choice == 'N')
    {
        cout << "Terminal is Closing" << endl;
    }
    return 0;
}
int chess()
{
// ===============================
// UNDO / REDO STORAGE
// ===============================

    int undoSrc[1000], undoDest[1000];
    char undoMoved[1000], undoCaptured[1000];
    char undoTurn[1000];
    int undoTop = -1;

    int redoSrc[1000], redoDest[1000];
    char redoMoved[1000], redoCaptured[1000];
    char redoTurn[1000];
    int redoTop = -1;

    //BOARD OF CHESS 
    char board[64];
    for (int i = 0; i < 64; i++)
        board[i] = '.';

    char black[8] = { 'r','n','b','q','k','b','n','r' };
    char white[8] = { 'R','N','B','Q','K','B','N','R' };

    for (int i = 0; i < 8; i++) {
        board[i] = black[i];
        board[i + 8] = 'p';
        board[i + 48] = 'P';
        board[i + 56] = white[i];
    }

    char turn = 'w';   // w = white, b = black

    // ===============================
    // GAME LOOP
    // ===============================
    while (1) 
    {

        // ===============================
        // DISPLAY BOARD
        // ===============================
        cout << "\n    A   B   C   D   E   F   G   H\n";
        cout << "  +---+---+---+---+---+---+---+---+\n";
        for (int i = 0; i < 64; i++) {
            if (i % 8 == 0)
                cout << 8 - (i / 8) << " | ";
            cout << board[i] << " | ";
            if ((i + 1) % 8 == 0)
                cout << "\n  +---+---+---+---+---+---+---+---+\n";
        }

        // ===============================
        // FIND KING POSITION
        // ===============================
        char kingChar = (turn == 'w') ? 'K' : 'k';
        int kingPos = -1;

        for (int i = 0; i < 64; i++)
        {
            if (board[i] == kingChar)
            {
                kingPos = i;
                break;
            }
        }

        // ===============================
        // CHECK DETECTION
        // ===============================
        int check = 0;

        for (int i = 0; i < 64; i++) 
        {
            if (board[i] == '.') continue;

            int enemyWhite = (board[i] >= 'A' && board[i] <= 'Z');
            int kingWhite = (turn == 'w');

            if (enemyWhite != kingWhite) 
            {

                int sr = i / 8, sc = i % 8;
                int dr = kingPos / 8, dc = kingPos % 8;
                int rd = dr - sr, cd = dc - sc;
                int ar = rd < 0 ? -rd : rd;
                int ac = cd < 0 ? -cd : cd;

                char p = board[i];

                // Pawn
                if ((p == 'p' && rd == 1 && ac == 1) ||
                    (p == 'P' && rd == -1 && ac == 1))
                    check = 1;

                // Knight
                if ((p == 'n' || p == 'N') &&
                    ((ar == 2 && ac == 1) || (ar == 1 && ac == 2)))
                    check = 1;

                // King
                if ((p == 'k' || p == 'K') && ar <= 1 && ac <= 1)
                    check = 1;

                // Rook / Queen (Straight)
                if ((p == 'r' || p == 'R' || p == 'q' || p == 'Q') &&
                    (sr == dr || sc == dc)) {

                    int rStep = (rd > 0) ? 1 : (rd < 0 ? -1 : 0);
                    int cStep = (cd > 0) ? 1 : (cd < 0 ? -1 : 0);

                    int r = sr + rStep, c = sc + cStep;
                    int blocked = 0;

                    while (r != dr || c != dc) {
                        if (board[r * 8 + c] != '.') blocked = 1;
                        r += rStep;
                        c += cStep;
                    }

                    if (blocked == 0) check = 1;
                }

                // Bishop / Queen (Diagonal)
                if ((p == 'b' || p == 'B' || p == 'q' || p == 'Q') &&
                    ar == ac) {

                    int rStep = (rd > 0) ? 1 : -1;
                    int cStep = (cd > 0) ? 1 : -1;

                    int r = sr + rStep, c = sc + cStep;
                    int blocked = 0;

                    while (r != dr || c != dc) 
                    {
                        if (board[r * 8 + c] != '.') blocked = 1;
                        r += rStep;
                        c += cStep;
                    }

                    if (blocked == 0) check = 1;
                }
            }
        }

        if (check == 1)
            cout << "\nWARNING: KING IS IN CHECK!\n";
// CHECKMATE DETECTION 
        if (check == 1)
        {

            int escape = 0; // Flag to see if any move saves king

            // Loop over all 64 squares (source)
            for (int src = 0; src < 64 && escape == 0; src++) 
            {
                if (board[src] == '.') continue;

                int pieceWhite = (board[src] >= 'A' && board[src] <= 'Z');
                int turnWhite = (turn == 'w');
                if (pieceWhite != turnWhite) continue; // Only current player's pieces

                // Try all 64 squares as destination
                for (int dest = 0; dest < 64 && escape == 0; dest++)
                {
                    if (src == dest) continue;

                    char target = board[dest];
                    if (target != '.') {
                        int targetWhite = (target >= 'A' && target <= 'Z');
                        if (targetWhite == pieceWhite) continue; // Cannot capture own piece
                    }

                    // --- Compute row/col differences ---
                    int sr = src / 8, sc = src % 8;
                    int dr = dest / 8, dc = dest % 8;
                    int rd = dr - sr, cd = dc - sc;
                    int ar = rd < 0 ? -rd : rd;
                    int ac = cd < 0 ? -cd : cd;

                    char piece = board[src];
                    int legal = 0;

                    // --- Full move validation (no jumping) ---
                    switch (piece) {
                    case 'P':
                        if (cd == 0 && rd == -1 && target == '.') legal = 1;
                        if (sr == 6 && cd == 0 && rd == -2 && target == '.' && board[src - 8] == '.') legal = 1;
                        if (ar == 1 && rd == -1 && target != '.') legal = 1;
                        break;
                    case 'p':
                        if (cd == 0 && rd == 1 && target == '.') legal = 1;
                        if (sr == 1 && cd == 0 && rd == 2 && target == '.' && board[src + 8] == '.') legal = 1;
                        if (ar == 1 && rd == 1 && target != '.') legal = 1;
                        break;
                    case 'N': case 'n':
                        if ((ar == 2 && ac == 1) || (ar == 1 && ac == 2)) legal = 1;
                        break;
                    case 'K': case 'k':
                        if (ar <= 1 && ac <= 1) legal = 1;
                        break;
                    case 'R': case 'r':
                        if (sr == dr || sc == dc)
                        {
                            int rs = (rd > 0) ? 1 : (rd < 0 ? -1 : 0);
                            int cs = (cd > 0) ? 1 : (cd < 0 ? -1 : 0);
                            int r = sr + rs, c = sc + cs;
                            legal = 1;
                            while (r != dr || c != dc)
                            {
                                if (board[r * 8 + c] != '.') { legal = 0; break; }
                                r += rs; c += cs;
                            }
                        }
                        break;
                    case 'B': case 'b':
                        if (ar == ac)
                        {
                            int rs = (rd > 0) ? 1 : -1;
                            int cs = (cd > 0) ? 1 : -1;
                            int r = sr + rs, c = sc + cs;
                            legal = 1;
                            while (r != dr || c != dc) 
                            {
                                if (board[r * 8 + c] != '.') { legal = 0; break; }
                                r += rs; c += cs;
                            }
                        }
                        break;
                    case 'Q': case 'q':
                        if (sr == dr || sc == dc || ar == ac) 
                        {
                            int rs = (rd > 0) ? 1 : (rd < 0 ? -1 : 0);
                            int cs = (cd > 0) ? 1 : (cd < 0 ? -1 : 0);
                            int r = sr + rs, c = sc + cs;
                            legal = 1;
                            while (r != dr || c != dc) {
                                if (board[r * 8 + c] != '.') { legal = 0; break; }
                                r += rs; c += cs;
                            }
                        }
                        break;
                    }

                    if (legal == 0) continue; // Skip illegal moves

                    // --- Make temporary move ---
                    char savedSrc = board[src];
                    char savedDest = board[dest];
                    board[dest] = board[src];
                    board[src] = '.';

                    // --- Check if king is safe after this move ---
                    char kChar = (turn == 'w') ? 'K' : 'k';
                    int kPos = -1;
                    for (int i = 0; i < 64; i++) if (board[i] == kChar) { kPos = i; break; }

                    int stillCheck = 0;
                    for (int i = 0; i < 64; i++) 
                    {
                        if (board[i] == '.') continue;
                        int enemyWhite = (board[i] >= 'A' && board[i] <= 'Z');
                        if (enemyWhite == turnWhite) continue;

                        int sr2 = i / 8, sc2 = i % 8;
                        int dr2 = kPos / 8, dc2 = kPos % 8;
                        int rd2 = dr2 - sr2, cd2 = dc2 - sc2;
                        int ar2 = rd2 < 0 ? -rd2 : rd2;
                        int ac2 = cd2 < 0 ? -cd2 : cd2;
                        char p = board[i];

                        // Pawn attack
                        if ((p == 'p' && rd2 == 1 && ac2 == 1) || (p == 'P' && rd2 == -1 && ac2 == 1)) stillCheck = 1;
                        // Knight attack
                        if ((p == 'n' || p == 'N') && ((ar2 == 2 && ac2 == 1) || (ar2 == 1 && ac2 == 2))) stillCheck = 1;
                        // King attack
                        if ((p == 'k' || p == 'K') && ar2 <= 1 && ac2 <= 1) stillCheck = 1;
                        // Rook/Queen straight
                        if ((p == 'r' || p == 'R' || p == 'q' || p == 'Q') && (sr2 == dr2 || sc2 == dc2))
                        {
                            int rs = (rd2 > 0) ? 1 : (rd2 < 0 ? -1 : 0);
                            int cs = (cd2 > 0) ? 1 : (cd2 < 0 ? -1 : 0);
                            int r = sr2 + rs, c = sc2 + cs;
                            while (r != dr2 || c != dc2) 
                            {
                                if (board[r * 8 + c] != '.') { rs = cs = 0; break; }
                                r += rs; c += cs;
                            }
                            if (r == dr2 && c == dc2) stillCheck = 1;
                        }
                        // Bishop/Queen diagonal
                        if ((p == 'b' || p == 'B' || p == 'q' || p == 'Q') && ar2 == ac2) 
                        {
                            int rs = (rd2 > 0) ? 1 : -1;
                            int cs = (cd2 > 0) ? 1 : -1;
                            int r = sr2 + rs, c = sc2 + cs;
                            while (r != dr2 || c != dc2)
                            {
                                if (board[r * 8 + c] != '.') { rs = cs = 0; break; }
                                r += rs; c += cs;
                            }
                            if (r == dr2 && c == dc2) stillCheck = 1;
                        }
                    }

                    // --- Undo temporary move ---
                    board[src] = savedSrc;
                    board[dest] = savedDest;

                    if (stillCheck == 0) escape = 1; // Found at least one move that saves king
                }
            }

            // --- If no moves can save king → checkmate ---
            if (escape == 0) 
            {
                if (turn == 'w') cout << "\nCHECKMATE! Black wins!\n";
                else cout << "\nCHECKMATE! White wins!\n";
                break; // End game
            }
        }



        // ===============================
        // PLAYER INPUT
        // ===============================
        cout << (turn == 'w' ? "White" : "Black") << " move (E2 E4, Q to quit, U to undo, R to redo): ";

        char cmd;
        cin >> cmd;

        // --- QUIT ---
        if (cmd == 'Q' || cmd == 'q') 
        {
            cout << "\nPlayer resigned. Game Over.\n";
            break;
        }

        // --- UNDO ---
        if (cmd == 'U' || cmd == 'u') 
        {
            if (undoTop < 0)
            {
                cout << "No moves to undo!\n";
            }
            else {
                // Save for redo
                redoTop++;
                redoSrc[redoTop] = undoSrc[undoTop];
                redoDest[redoTop] = undoDest[undoTop];
                redoMoved[redoTop] = undoMoved[undoTop];
                redoCaptured[redoTop] = undoCaptured[undoTop];
                redoTurn[redoTop] = undoTurn[undoTop];

                // Restore board
                board[undoSrc[undoTop]] = undoMoved[undoTop];
                board[undoDest[undoTop]] = undoCaptured[undoTop];
                turn = undoTurn[undoTop];

                undoTop--;
            }
            continue;
        }

        // --- REDO ---
        if (cmd == 'R' || cmd == 'r')
        {
            if (redoTop < 0)
            {
                cout << "No moves to redo!\n";
            }
            else 
            {
                // Save back to undo
                undoTop++;
                undoSrc[undoTop] = redoSrc[redoTop];
                undoDest[undoTop] = redoDest[redoTop];
                undoMoved[undoTop] = redoMoved[redoTop];
                undoCaptured[undoTop] = redoCaptured[redoTop];
                undoTurn[undoTop] = redoTurn[redoTop];

                // Apply move
                board[redoDest[redoTop]] = redoMoved[redoTop];
                board[redoSrc[redoTop]] = '.';

                // Switch turn
                turn = (redoTurn[redoTop] == 'w') ? 'b' : 'w';

                redoTop--;
            }
            continue;
        }

        // --- NORMAL MOVE INPUT ---
        char c2;
        int r1, r2;
        cin >> r1 >> c2 >> r2;

        int col1 = (cmd >= 'a') ? cmd - 'a' : cmd - 'A';
        int col2 = (c2 >= 'a') ? c2 - 'a' : c2 - 'A';
        int row1 = 8 - r1;
        int row2 = 8 - r2;

        if (col1 < 0 || col1 > 7 || col2 < 0 || col2 > 7 ||
            row1 < 0 || row1 > 7 || row2 < 0 || row2 > 7) {
            cout << "Invalid input!\n";
            continue;
        }

        int src = row1 * 8 + col1;
        int dest = row2 * 8 + col2;


        char piece = board[src];
        if (piece == '.') {
            cout << "No piece selected!\n";
            continue;
        }

        int pieceWhite = (piece >= 'A' && piece <= 'Z');
        if ((turn == 'w' && pieceWhite == 0) ||
            (turn == 'b' && pieceWhite == 1)) {
            cout << "Not your piece!\n";
            continue;
        }

        // ===============================
        // MOVE VALIDATION
        // ===============================
        int legal = 0;
        int sr = src / 8, sc = src % 8;
        int dr = dest / 8, dc = dest % 8;
        int rd = dr - sr, cd = dc - sc;
        int ar = rd < 0 ? -rd : rd;
        int ac = cd < 0 ? -cd : cd;

        char target = board[dest];
        if (target != '.') {
            int targetWhite = (target >= 'A' && target <= 'Z');
            if (targetWhite == pieceWhite) {
                cout << "Cannot capture own piece!\n";
                continue;
            }
        }

        switch (piece) 
        {

        case 'P':
            if (cd == 0 && rd == -1 && target == '.') legal = 1;
            if (sr == 6 && cd == 0 && rd == -2 && target == '.' && board[src - 8] == '.') legal = 1;
            if (ar == 1 && rd == -1 && target != '.') legal = 1;
            break;

        case 'p':
            if (cd == 0 && rd == 1 && target == '.') legal = 1;
            if (sr == 1 && cd == 0 && rd == 2 && target == '.' && board[src + 8] == '.') legal = 1;
            if (ar == 1 && rd == 1 && target != '.') legal = 1;
            break;

        case 'R': case 'r':
            if (sr == dr || sc == dc) {
                int rs = (rd > 0) ? 1 : (rd < 0 ? -1 : 0);
                int cs = (cd > 0) ? 1 : (cd < 0 ? -1 : 0);

                int r = sr + rs, c = sc + cs;
                legal = 1;

                while (r != dr || c != dc) {
                    if (board[r * 8 + c] != '.') {
                        legal = 0;
                        break;
                    }
                    r += rs;
                    c += cs;
                }
            }
            break;


        case 'B': case 'b':
            if (ar == ac) {
                int rs = (rd > 0) ? 1 : -1;
                int cs = (cd > 0) ? 1 : -1;

                int r = sr + rs, c = sc + cs;
                legal = 1;

                while (r != dr || c != dc) {
                    if (board[r * 8 + c] != '.') {
                        legal = 0;
                        break;
                    }
                    r += rs;
                    c += cs;
                }
            }
            break;


        case 'Q': case 'q':
            if (sr == dr || sc == dc || ar == ac) {
                int rs = (rd > 0) ? 1 : (rd < 0 ? -1 : 0);
                int cs = (cd > 0) ? 1 : (cd < 0 ? -1 : 0);

                int r = sr + rs, c = sc + cs;
                legal = 1;

                while (r != dr || c != dc) {
                    if (board[r * 8 + c] != '.') {
                        legal = 0;
                        break;
                    }
                    r += rs;
                    c += cs;
                }
            }
            break;

        case 'N': case 'n':
            if ((ar == 2 && ac == 1) || (ar == 1 && ac == 2)) legal = 1;
            break;

        case 'K': case 'k':
            if (ar <= 1 && ac <= 1) legal = 1;
            break;
        }

        if (legal == 0) {
            cout << "Illegal move!\n";
            continue;
        }
// ===============================
// SELF-CHECK PREVENTION (NEW CODE)
// ===============================

// Save original pieces
        char savedSource = board[src];
        char savedDest = board[dest];

        // Make temporary move
        board[dest] = board[src];
        board[src] = '.';

        // Re-check king safety
        int stillCheck = 0;
        int kPos = -1;
        char kChar = (turn == 'w') ? 'K' : 'k';

        // Find king
        for (int i = 0; i < 64; i++) {
            if (board[i] == kChar) {
                kPos = i;
                break;
            }
        }

        // Scan enemy pieces
        for (int i = 0; i < 64; i++) {
            if (board[i] == '.') continue;

            int enemyWhite = (board[i] >= 'A' && board[i] <= 'Z');
            int kingWhite = (turn == 'w');

            if (enemyWhite != kingWhite) {

                int sr2 = i / 8, sc2 = i % 8;
                int dr2 = kPos / 8, dc2 = kPos % 8;
                int rd2 = dr2 - sr2, cd2 = dc2 - sc2;
                int ar2 = rd2 < 0 ? -rd2 : rd2;
                int ac2 = cd2 < 0 ? -cd2 : cd2;

                char p2 = board[i];

                // Pawn attack
                if ((p2 == 'p' && rd2 == 1 && ac2 == 1) ||
                    (p2 == 'P' && rd2 == -1 && ac2 == 1))
                    stillCheck = 1;

                // Knight attack
                if ((p2 == 'n' || p2 == 'N') &&
                    ((ar2 == 2 && ac2 == 1) || (ar2 == 1 && ac2 == 2)))
                    stillCheck = 1;

                // King attack
                if ((p2 == 'k' || p2 == 'K') && ar2 <= 1 && ac2 <= 1)
                    stillCheck = 1;

                // Rook / Queen
                if ((p2 == 'r' || p2 == 'R' || p2 == 'q' || p2 == 'Q') &&
                    (sr2 == dr2 || sc2 == dc2)) {

                    int rs = (rd2 > 0) ? 1 : (rd2 < 0 ? -1 : 0);
                    int cs = (cd2 > 0) ? 1 : (cd2 < 0 ? -1 : 0);

                    int r = sr2 + rs, c = sc2 + cs;
                    int blocked = 0;

                    while (r != dr2 || c != dc2) {
                        if (board[r * 8 + c] != '.') blocked = 1;
                        r += rs;
                        c += cs;
                    }

                    if (blocked == 0) stillCheck = 1;
                }

                // Bishop / Queen
                if ((p2 == 'b' || p2 == 'B' || p2 == 'q' || p2 == 'Q') &&
                    ar2 == ac2) {

                    int rs = (rd2 > 0) ? 1 : -1;
                    int cs = (cd2 > 0) ? 1 : -1;

                    int r = sr2 + rs, c = sc2 + cs;
                    int blocked = 0;

                    while (r != dr2 || c != dc2) {
                        if (board[r * 8 + c] != '.') blocked = 1;
                        r += rs;
                        c += cs;
                    }

                    if (blocked == 0) stillCheck = 1;
                }
            }
        }

        // If king still in check → undo move
        if (stillCheck == 1) {
            board[src] = savedSource;
            board[dest] = savedDest;
            cout << "Illegal move! King still in check.\n";
            continue;
        }

        // Board already contains the final move at this point:
        // board[dest] == savedSource, board[src] == '.'

        // SAVE MOVE FOR UNDO (use the saved variables!)
        undoTop++;
        undoSrc[undoTop] = src;
        undoDest[undoTop] = dest;
        undoMoved[undoTop] = savedSource;   // moved piece
        undoCaptured[undoTop] = savedDest;  // captured piece or '.'
        undoTurn[undoTop] = turn;

        // Clear redo history
        redoTop = -1;

        // Switch turn
        turn = (turn == 'w') ? 'b' : 'w';


    }
    system("pause");
    return 0;
}