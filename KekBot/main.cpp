#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <sys/time.h>
#include <vector>
#include <set>
#include "utils.h"

std::string myRecv(int fd) {
	std::string res;
	char buff[1000];
	int n = 0;
	while ((n = recv(fd, buff, 1000, 0))) {
		res += buff;
		bzero(buff, sizeof(buff));
		if (n < 1000) {
			break;
		}
		while((n = recv(fd, buff, 1000, 0)) == -1) {
			usleep(5);
		}
	}
	return res;
}

std::string connect_msg(const std::string& bot_name, const std::string& password) {
	if (password.empty())
		return "NICK " + bot_name + "\r\nUSER " + bot_name + " * * *\r\n";
	return "PASS " + password + "\r\n" + "NICK " + bot_name + "\r\nUSER " + bot_name + " * * *\r\n";
}

size_t getUniqueIndex(size_t index, size_t size) {
	size_t res;
	if (index == UINTMAX_MAX) {
		return rand() % size;
	}
	while ((res = rand() % size) == index);
	return res;
}

void reConnect(struct sockaddr_in& stSockAddr, int& nClientSock, int port) {
	do{
		nClientSock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
		if (nClientSock == -1) {
			throw std::runtime_error(strerror(errno));
		}
		stSockAddr.sin_family = PF_INET;
		stSockAddr.sin_port = htons(port);
		stSockAddr.sin_addr.s_addr = 0; // localhost
		sleep(1);
		if(-1 != connect(nClientSock, (struct sockaddr*) &stSockAddr, sizeof (stSockAddr))){
			break;
		}
		else
			close(nClientSock);
	}
	while (1);
}

int main(int ac, char **av) {
	if (ac < 2) {
		std::cout << "usage \"./bin port [pass]\"" << std::endl;
		exit(1);
	}
	struct sockaddr_in stSockAddr;
	int nClientSock;
	reConnect(stSockAddr, nClientSock, atoi(av[1]));

	char buff[1000];
	std::string res = connect_msg("kekBot", ac > 2 ? av[2] : "");
	write(nClientSock, res.c_str(), res.size());
	recv(nClientSock, buff, 1000, 0);
	res = buff;
	if (res.find("001") != std::string::npos || res.find("376") != std::string::npos) {
		std::cout << "okkkkkkkkkkk" << std::endl;
	} else {
		std::cout << "fail connect in server\n" << buff << std::endl;
		exit(1);
	}
	std::cout << buff << std::endl;
	fd_set forRead, forWrite;

	std::vector<std::string> jokes;
	jokes.push_back("— Почему мужчина открывает и закрывает женщине дверь в машине? Скажете, джентльмен?\n"
					"— Пф-ф-ф… Чтоб дверью не хлопала!");
	jokes.push_back("Купил мужик в магазине шляпу, а она ему как раз!");
	jokes.push_back("Увидел медведь машина горит, сел в нее и сгорел");
	jokes.push_back("XP за irc такая же как за трансенденс");
	jokes.push_back("Почему я люблю шутки Махмуда? У него всегда черный юмор!");
	jokes.push_back("Колобок повесился!");
	jokes.push_back("Лопата");
	jokes.push_back("Чем эта шутка похожа на мой джонсон? Она короткая");
	jokes.push_back("Все работает");
	jokes.push_back("— Почему мужчина открывает и закрывает женщине дверь в машине? Скажете, джентльмен?\n— Пф-ф-ф… Чтоб дверью не хлопала!");
	jokes.push_back("Сказочка на ночь: злая колдунья предсказала, что принцесса умрет, уколов палец веретеном. Король решил спасти свою дочь и отрезал ей все пальцы.");
	jokes.push_back("Один родитель, когда его спросили, что он может сделать для школы, сказал: \"Возбудить уголовное дело! \". Следующие 11 лет администрация не беспокоила его просьбами о материальной помощи.");
	jokes.push_back("В доме, где проживают сплошь одни интеллигенты, один доцент, в темноте стукнулся о дверной косяк и обронил слово « [м]ля». С тех пор он стал считаться местным криминальным авторитетом.");
	jokes.push_back("— Кто вчера смотрел фильм ужасов?\n— Я!\n— Иди и убери за собой!");
	jokes.push_back("Приходит девушка на работу в темных очках, лицо побито, синяки под глазами.\nПодруга спрашивает:\n— Что случилось, Анжелочка?\n— Ты понимаешь, вчера лежу я со своим, а тут приходит мой!");
	jokes.push_back("— Зачем вы украли рулон туалетной бумаги?\n— Я не крал. А собирался рассчитаться позже.\n— У вас не было денегй, чтобы заплатить?\n— У меня не было времени, чтобы успеть заплатить!");
	jokes.push_back("На уроке.\n— Дети, какие вы знаете народные приметы про птиц? Да, Машенька?\n"
					"— Если воробьи купаются в пыли, то будет дождь.\n— Молодец, Машенька. А ты, Петя?\n"
					"— Если чайка низко, значит, буря близко. Если чайка высоко, значит, буря далеко.\n"
					"— Какой ты молодец, Петенька! Вовочка, а ты какую примету знаешь?\n"
					"— Чайка на трибуне — сенатора посадят.");
	jokes.push_back("Турист подходит к фермеру:\n"
					"— Извините, не разрешите ли Вы мне пройти через ваше поле. Иначе я не успею на последний поезд. — Иди, конечно. А если мой бык тебя увидит, то успеешь и на предыдущий.");
	jokes.push_back("Пишу смс подруге:\n"
					"— Еду к тебе в гости! Чайку приготовишь?\n"
					"Ее ответ убил:\n"
					"— Где я тебе ее возьму? Давай курицу приготовлю!");
	jokes.push_back("– Я пристаю, когда пьяный… — сказал он, смущаясь. Она молча достала бутылку и стакан.");
	jokes.push_back("Голубей хлебом не корми, хлебом корми");
	jokes.push_back("У меня такой ИНН, что тебе не СНИЛС");
	jokes.push_back("Да что вы такое говорите?! Не бил я её! Я просто “дал пять” по лицу.");
	jokes.push_back("— Приветик, дурак! — Сам ты приветик.");
	jokes.push_back("Толстые стриптизерши иногда перегибают палку.");
	jokes.push_back("Почему в стриптизерши не берут толстых.");
	jokes.push_back("ну что мне показалось забавным сказал Миша, Очень старая никогда не была женой путина, демоны вошли в душу. но прикольно даже не это");
	srand(time(0x0));
	size_t oldIndex = -1, index = rand() % jokes.size();
    signal(SIGPIPE, SIG_IGN);
	volatile bool loop = true;
	struct timeval tv, current;
	gettimeofday(&current, 0x0);
	std::set<std::string> channels;
	FD_SET(nClientSock, &forWrite);
	while (loop) {
		bzero(&forRead, sizeof(forRead));

		FD_SET(nClientSock, &forRead);
		if (select(nClientSock + 1, &forRead, &forWrite, 0x0, 0x0) < 0) {
			continue;
		}
		gettimeofday(&tv, 0x0);
		if (tv.tv_sec - current.tv_sec > 5) {
			gettimeofday(&current, 0x0);
			int kek = write(nClientSock, "LIST\r\n", 6);
			if (kek == -1) {
				reConnect(stSockAddr, nClientSock, atoi(av[1]));
				channels.clear();
				res = connect_msg("kekBot", ac > 2 ? av[2] : "");
				write(nClientSock, res.c_str(), res.size());

				std::cout << "++++++++++++++++++++\n" << recv(nClientSock, buff, 1000, 0) << "\n++++++++++++++++++++\n";
			}
		}
		if (FD_ISSET(nClientSock, &forRead)) {
			res = myRecv(nClientSock);
			std::cout << "-----------------------\n" << res << "\n---------------------\n";
			if (res.find(":SPAM\r\n") != std::string::npos) {
				int start = res.find('#');
				int end = res.find_first_of(' ', start);
				std::string cmd = res.substr(start, end - start);
				index = getUniqueIndex(oldIndex, jokes.size());
				std::vector<std::string> splitted = ft::split(jokes[index], '\n');
				for(size_t i = 0; i < splitted.size(); i++) {
					res = "PRIVMSG " + cmd + " :" + splitted[i] + "\r\n";
					write(nClientSock, res.c_str(), res.size());
				}
				oldIndex = index;
			} else if (res.find("LIST") != std::string::npos) {
				std::vector<std::string> vCmd = ft::split(res, "\r\n");
				if (vCmd.size() > 2) {
					for (size_t i = 1; i < vCmd.size() - 1; ++i) {
						int start = vCmd[i].find('#');
						int end = vCmd[i].find_first_of(' ', start);
						if (start == -1) {
							continue;
						}
						std::string cmd = vCmd[i].substr(start, end - start);
						if (!channels.count(cmd)) {
							res = "JOIN " + cmd + "\r\n";
							write(nClientSock, res.c_str(), res.size());
							res = myRecv(nClientSock);
							if (res.find("353") != std::string::npos)
								channels.insert(cmd);
						}
					}
				}
			} else if (res.empty()) {
				sleep(5);
			}
		}
//        bzero(&forWrite, sizeof(forWrite));

    }


	close(nClientSock);
	return 0;
}

//PASS 123
//NICK HH
//USER HH H H H
//JOIN #J