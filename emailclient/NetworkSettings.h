#ifndef _NETWORK_SETTING_H
#define _NETWORK_SETTING_H

// Network credentials
constexpr char WIFI_SSID[] {"WIFI_SSID"};
constexpr char WIFI_PASSWORD[] {"WIFI_PASSWORD"};
constexpr char NTP_SERVER[] {"NTP_SERVER"};

// Sender email credentials (use the App Password here)
constexpr char AUTHOR_EMAIL[] {"AUTHOR_EMAIL"};
constexpr char AUTHOR_PASSWORD[] {"AUTHOR_PASSWORD"};

// Recipient email
constexpr char RECIPIENT_EMAIL_1[] {"RECIPIENT_EMAIL_1"};
constexpr char RECIPIENT_EMAIL_2[] {"RECIPIENT_EMAIL_2"};
constexpr char RECIPIENT_EMAIL_3[] {"RECIPIENT_EMAIL_3"};

// SMTP server details (for Gmail)
constexpr char SMTP_SERVER[] {"smtp.gmail.com"};
constexpr int SMTP_PORT {465};

#endif