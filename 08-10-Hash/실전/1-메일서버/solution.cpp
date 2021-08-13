using hash_t = unsigned long long;
constexpr hash_t FNV_BASIS{0xcbf29ce484222325ULL};
constexpr hash_t FNV_PRIME{0x100000001b3ULL};

hash_t fnv1a(const char *str)
{
    hash_t hash{FNV_BASIS};
    while (*str) {
        hash ^= *str++;
        hash *= FNV_PRIME;
    }
    return hash;
}

hash_t fnv1a(const char *str, int len)
{
    hash_t hash{FNV_BASIS};
    for (int i{0}; i < len; ++i) {
        hash ^= str[i];
        hash *= FNV_PRIME;
    }
    return hash;
}

constexpr int MAX_USERS{1000};
constexpr int MAX_MAILS{10000};
constexpr int MAX_INBOX{300};
constexpr int MAX_WORD_CNT{10};
constexpr int MAX_SUBJECT{200};

int users_size;
int inbox_size;

struct Mail {
    hash_t subject;
    hash_t words[MAX_WORD_CNT];
    int word_cnt;
} mail_nodes[MAX_MAILS];
int mail_node_cnt{0};
hash_t word_buf[MAX_WORD_CNT];
int mail_alloc(hash_t subject, int word_cnt)
{
    Mail &mail = mail_nodes[mail_node_cnt];
    mail.subject = subject;
    mail.word_cnt = word_cnt;
    for (int i{0}; i < word_cnt; ++i) {
        mail.words[i] = word_buf[i];
    }
    return mail_node_cnt++;
}

struct MailWrapper {
    int mail;
    int next;
} wrapper_nodes[MAX_USERS * MAX_MAILS];
int wrapper_node_cnt{0};
int wrapper_alloc(int mail, int next)
{
    wrapper_nodes[wrapper_node_cnt].mail = mail;
    wrapper_nodes[wrapper_node_cnt].next = next;
    return wrapper_node_cnt++;
}

struct Inbox {
    int wrapper;
    int mail_cnt;

    void add(int mail)
    {
        wrapper = wrapper_alloc(mail, wrapper);
        if (mail_cnt < inbox_size) {
            ++mail_cnt;
        }
    }
} inboxes[MAX_USERS];

void init(int n, int k)
{
    users_size = n;
    inbox_size = k;

    mail_node_cnt = wrapper_node_cnt = 0;
    for (int user{0}; user < users_size; ++user) {
        inboxes[user].wrapper = inboxes[user].mail_cnt = 0;
    }
}

void sendMail(char *subject, int sender, int cnt, int *receivers)
{
    (void)sender;
    const hash_t hash{fnv1a(subject)};

    const char *word{subject};
    int word_cnt{0};
    int word_len{0};
    while (true) {
        ++word_len;
        ++subject;
        if (*subject == ' ') {
            word_buf[word_cnt++] = fnv1a(word, word_len);
            word_len = 0;
            word = ++subject;
        } else if (!*subject) {
            word_buf[word_cnt++] = fnv1a(word, word_len);
            break;
        }
    }

    const int mail{mail_alloc(hash, word_cnt)};
    for (int i{0}; i < cnt; ++i) {
        inboxes[receivers[i]].add(mail);
    }
}

int getCount(int user) { return inboxes[user].mail_cnt; }

int deleteMail(int user, char *subject)
{
    const hash_t hash{fnv1a(subject)};
    Inbox &inbox = inboxes[user];
    int del_cnt{0};

    int *wrapper{&inbox.wrapper};
    int i{inbox.mail_cnt};
    while (i--) {
        const int mail{wrapper_nodes[*wrapper].mail};
        const int w{*wrapper};
        if (mail_nodes[mail].subject == hash) {
            ++del_cnt;
            *wrapper = wrapper_nodes[w].next;
        }
        wrapper = &wrapper_nodes[w].next;
    }

    inbox.mail_cnt -= del_cnt;
    return del_cnt;
}

int searchMail(int user, char *text)
{
    const hash_t hash{fnv1a(text)};
    Inbox &inbox = inboxes[user];
    int fnd_cnt{0};

    int wrapper{inbox.wrapper};
    int i{inbox.mail_cnt};
    while (i--) {
        const Mail &mail = mail_nodes[wrapper_nodes[wrapper].mail];
        for (int j{0}; j < mail.word_cnt; ++j) {
            if (mail.words[j] == hash) {
                ++fnd_cnt;
                break;
            }
        }
        wrapper = wrapper_nodes[wrapper].next;
    }

    return fnd_cnt;
}
