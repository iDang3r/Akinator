//
// Created by Александр on 09.11.2019.
//

#ifndef AKINATOR_AKINATOR_TREE_H
#define AKINATOR_AKINATOR_TREE_H

#include "Tree.h"
#include "my_file_lib.h"
#include "color_printf.h"
#include "super_define.h"

//typedef char* str_t;
#define str_t char*

const char open_bracket  = '{';
const char close_bracket = '}';
const char nil_node[]    = "nil";

const char voice_command[] = "say ";

struct Akinator_Tree : Tree<str_t> {

    bool edited = false;

    bool play_again = false;

    // Methods

    void free_str_s_rec(node<str_t>* el) {
        assert(el != nullptr);

        free(el->data_);

        if (el->left_ != nullptr)
            free_str_s_rec(el->left_);

        if (el->right_ != nullptr)
            free_str_s_rec(el->right_);
    }

    ~Akinator_Tree() {
        cout << "Akinator_Tree was destructed" << endl;
        if (head_ != nullptr) {
            free_str_s_rec(head_);
        }
    }

    void push_head_s(const str_t s) {
        char* tmp = (char*)calloc(strlen(s) + 1, sizeof(char));
        strcpy(tmp, s);
        push_head(tmp);
    }

    void push_left_s(node<str_t>* el, const str_t s) {
        char* tmp = (char*)calloc(strlen(s) + 1, sizeof(char));
        strcpy(tmp, s);
        push_left(el, tmp);
    }

    void push_right_s(node<str_t>* el, const str_t s) {
        char* tmp = (char*)calloc(strlen(s) + 1, sizeof(char));
        strcpy(tmp, s);
        push_right(el, tmp);
    }

    void dump_picture_rec(FILE* out, node<str_t>* el, int last = -1) {

        fprintf(out, "class %p {\n", el);

        if (last == 1) {
            fprintf(out, "YES\n");
        } else if (last == 0) {
            fprintf(out, "NO\n");
        } else {
            fprintf(out, "Default\n");
        }

        fprintf(out, "--\n");

        fprintf(out, "\"%s\"\n", el->data_);

        if (el->right_) {
            fprintf(out, "--\n");
            fprintf(out, "yes: %p\n", el->right_);
        }

        if (el->left_) {
            fprintf(out, "--\n");
            fprintf(out, "no: %p\n", el->left_);
        }

        fprintf(out, "}\n");

        if (el->right_) {
            fprintf(out, "%p -down-> %p\n", el, el->right_);

            dump_picture_rec(out, el->right_, 1);
        }

        if (el->left_) {
            fprintf(out, "%p -down-> %p\n", el, el->left_);

            dump_picture_rec(out, el->left_, 0);
        }
    }

    void dump_picture() {

        FILE* out = fopen("/Users/alex/Desktop/pictures/Akinator_uml.pu", "w");

        if (out == nullptr) {
            dump(DUMP_INFO, "file was not opened");
            return;
        }

        fprintf(out, "@startuml\n !define DARKORANGE\n !include style.puml\n class head_\n");

        fprintf(out, "head_ -down-> %p\n", head_);

        dump_picture_rec(out, head_);

        fprintf(out, "@enduml\n");
        fclose(out);

        system("java -jar --illegal-access=warn /Users/alex/plantuml.jar -tsvg /Users/alex/Desktop/pictures/Akinator_uml.pu");
        system("open /Users/alex/Desktop/pictures/Akinator_uml.svg");
    }

    char* skip_spaces(char* buff) {
        int n = 0;
        sscanf(buff, "%*[ \t]%n", &n);
        return buff + n;
    }

    char* build_tree_rec(char* buff, node<str_t>* *el, char* str_buff) {
        assert(el != nullptr);
        assert(buff != nullptr);
        assert(str_buff != nullptr);

        buff = skip_spaces(buff);

        assert(*buff == open_bracket);
        buff++;
        int n = 0;

        buff = skip_spaces(buff);

        sscanf(buff, "'%[^']'%n", str_buff, &n);
        buff += n;
        char* new_str = (char*)calloc(strlen(str_buff) + 1, sizeof(char));

        if (new_str == nullptr) {
            dump(DUMP_INFO, "memory was not allocated");
            assert(ERROR);
        }

        strcpy(new_str, str_buff);
        *el = make_node(new_str);

        buff = skip_spaces(buff);

        if (*buff == close_bracket)
            return buff + 1;


        buff = skip_spaces(buff);

        if (strncmp(buff, nil_node, strlen(nil_node)) == 0) {
            buff += strlen(nil_node);
        } else {
            buff = build_tree_rec(buff, &((*el)->left_), str_buff);
        }

        buff = skip_spaces(buff);

        if (strncmp(buff, nil_node, strlen(nil_node)) == 0) {
            buff += strlen(nil_node);
        } else {
            buff = build_tree_rec(buff, &((*el)->right_), str_buff);
        }

        buff = skip_spaces(buff);

        assert(*buff == close_bracket);
        return buff + 1;
    }

    int get_tree_from_file(const char* name_of_file) {
        if (head_ != nullptr) {
            dump(DUMP_INFO, "head_ not null");
            free_str_s_rec(head_);
            free_tree(head_);
            head_ = nullptr;
        }

        char* buff = nullptr;
        size_t buff_size = get_buffer(name_of_file, &buff);

        char str_buff[128] = {};

        build_tree_rec(buff, &head_, str_buff);

        free(buff);

        return 0;
    }


    void put_tree_rec(FILE* out, node<str_t>* el) {
        assert(out);
        assert(el);

        fprintf(out, "%c", open_bracket);

        fprintf(out, "'%s'", el->data_);

        if (el->left_ != nullptr || el->right_ != nullptr) {
            if (el->left_ != nullptr) {
                put_tree_rec(out, el->left_);
            } else {
                fprintf(out, "%s ", nil_node);
            }

            if (el->right_ != nullptr) {
                put_tree_rec(out, el->right_);
            } else {
                fprintf(out, "%s", nil_node);
            }
        }

        fprintf(out, "%c", close_bracket);
    }

    int put_tree_in_file(const char* name_of_file) {
        FILE* out = fopen(name_of_file, "w");

        if (out == nullptr) { // обязательна ли эта проверка
            dump(DUMP_INFO, "file was not opened");
            return 1;
        }

        if (head_ != nullptr)
            put_tree_rec(out, head_);
        else
            fprintf(out, "%s", nil_node);

        fclose(out);
        printf("Tree was rewrited");
        return 0;
    }

    void print_and_say(const str_t s) {
        char* command = concat(voice_command, s);
        printf("%s", s);
        fflush(stdout);
        system(command);
        free(command);
    }

    void ask_yes_no(char* s) {
        scanf("%s", s);
        while (strcmp(s, "нет") && strcmp(s, "да") &&
               strcmp(s, "no") && strcmp(s, "yes")) {
            print_and_say("Вы можете отвечать только 'да' или 'нет'!\n");
            scanf("%s", s);
        }
    }

    bool check_for_same_str(node<str_t>* el, const char* s) {
        if (strcmp(el->data_, s) == 0)
            return true;

        bool u = false;

        if (el->left_ != nullptr)
            u = check_for_same_str(el->left_, s);

        if (!u && el->right_ != nullptr)
            u = check_for_same_str(el->right_, s);

        return u;
    }

    bool get_definition(node<str_t>* el, const char* s) {
        if (strcmp(el->data_, s) == 0)
            return true;

        if (el->left_ != nullptr && get_definition(el->left_, s)) {
            char* neg_ans = concat("не ", el->data_);
            print_and_say(neg_ans);
            printf("\n");
            free(neg_ans);
            return true;
        }

        if (el->right_ != nullptr && get_definition(el->right_, s)) {
            print_and_say(el->data_);
            printf("\n");
            return true;
        }

        return false;
    }

    void ask_for_another_game(char* str_buff) {
        print_and_say("Хочешь сыграть ещё? ");
        ask_yes_no(str_buff);
        if (strcmp(str_buff, "no") == 0 || strcmp(str_buff, "нет") == 0) {
            print_and_say("Жаль, увидимся позже\n");
            play_again = false;
        } else {
            print_and_say("Ничего себе, какой ненасытный, ну, давай, сыграем ещё!\n");
            play_again = true;
        }
    }

    void gaming_rec(node<str_t>* el, str_t str_buff) {
        if (el->left_ == nullptr && el->right_ == nullptr) {
            print_and_say("Я думаю, что ваш персонаж это..... ");
            printf("\033[1;32m");
            print_and_say(el->data_);
            printf("\033[0m\n");

            print_and_say("Это ваш персонаж? ");
            ask_yes_no(str_buff);

            if (strcmp(str_buff, "no") == 0 || strcmp(str_buff, "нет") == 0) {
                // TODO

                print_and_say("Жаль, кого вы загадали? ");
                scanf("\n%[^\n]", str_buff);

                if (check_for_same_str(head_, str_buff)) {
                    print_and_say("К сожалению, характеристика этого персонажа не совпадает с вашими ответами. упсссс\n");
                    print_and_say("Вот правильная характеристика:\n");
                    get_definition(head_, str_buff);
                    ask_for_another_game(str_buff);
                    return;
                }

                char* new_answer = (char*)calloc(strlen(str_buff) + 1, sizeof(char));
                if (new_answer == nullptr) {                // насколько корректно программа должна на такое реагировать и продолжать работу
                    dump(DUMP_INFO, "memory was not allocated");
                    assert(ERROR);
                }

                strcpy(new_answer, str_buff);

                print_and_say("Какое качество есть у вашего персонажа, отличающее его от моего? ");

                scanf("\n%[^\n]", str_buff);

                while (strstr(str_buff, "не ") || strstr(str_buff, "not ")) {
                    print_and_say("Качество не должно содеражать частицы отрицания\n");
                    scanf("\n%[^\n]", str_buff);
                }

                char* new_question = (char*)calloc(strlen(str_buff) + 1, sizeof(char));
                if (new_question == nullptr) {
                    dump(DUMP_INFO, "memory was not allocated");
                    assert(ERROR);
                }

                strcpy(new_question, str_buff);

                push_left_s(el, el->data_);
                push_right_s(el, new_answer);
                el->data_ = new_question;

                edited = true;

            } else {
                print_and_say("Ещё бы! Меня ещё никто и никогда не обыгрывал!\n");
            }

            ask_for_another_game(str_buff);

            return;
        }

        char* question = concat(el->data_, "?  ");
        print_and_say(question);
        free(question);

        ask_yes_no(str_buff);

        if (strcmp(str_buff, "no") == 0 || strcmp(str_buff, "нет") == 0) {
            gaming_rec(el->left_, str_buff);
        } else {
            gaming_rec(el->right_, str_buff);
        }

    }

    void game() {
        print_and_say("Привет, мой друг, я Акинатор и хочу сыграть с тобой в одну игру!\n");
        print_and_say("Загадай персонажа, а я попробую его отгадать по вопросам!\n");
        print_and_say("Отвечай 'да' или 'нет'\n");

        char str_buff[32] = {};
        gaming_rec(head_, str_buff);

        while (play_again)
            gaming_rec(head_, str_buff);

    }

};

#endif //AKINATOR_AKINATOR_TREE_H
