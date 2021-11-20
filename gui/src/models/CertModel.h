#ifndef CERTMODEL_H
#define CERTMODEL_H

#include <QStandardItemModel>
#include <QStandardItem>

/*
 key->appendRow({new QStandardItem("Exponent"), key_exponent});
 key->appendRow({new QStandardItem("Modulus"), key_modulus});
 */
class CertModel{
public:
    CertModel():
        model(new QStandardItemModel),
        /* ... */
        info(new QStandardItem("Info")),
        fingerprint(new QStandardItem("Fingerprint")),
        issuer(new QStandardItem("Issuer")),
        validity(new QStandardItem("Validity")),
        subject(new QStandardItem("Subject")),
        key(new QStandardItem("Key")),
        subjectAltNames(new QStandardItem("Subject Alternative Names")),
        /* info */
        info_verison(new QStandardItem),
        info_signatureAlgorithm(new QStandardItem),
        info_serialNumber(new QStandardItem),
        /* fingerprint */
        fingerprint_md5(new QStandardItem),
        fingerprint_sha1(new QStandardItem),
        fingerprint_sha256(new QStandardItem),
        /* issuer */
        issuer_commonName(new QStandardItem),
        issuer_organizationName(new QStandardItem),
        issuer_countryName(new QStandardItem),
        issuer_localityName(new QStandardItem),
        issuer_stateOrProvinceName(new QStandardItem),
        issuer_email(new QStandardItem),
        /* validity */
        validity_notBefore(new QStandardItem),
        validity_notAfter(new QStandardItem),
        /* subject */
        subject_commonName(new QStandardItem),
        subject_organizationName(new QStandardItem),
        subject_localityName(new QStandardItem),
        subject_stateOrProvinceName(new QStandardItem),
        subject_countryName(new QStandardItem),
        subject_email(new QStandardItem),
        /* key */
        key_type(new QStandardItem),
        key_algorithm(new QStandardItem)
    {
        info->setForeground(QColor(220,220,220));
        fingerprint->setForeground(QColor(220,220,220));
        issuer->setForeground(QColor(220,220,220));
        validity->setForeground(QColor(220,220,220));
        subject->setForeground(QColor(220,220,220));
        key->setForeground(QColor(220,220,220));
        subjectAltNames->setForeground(QColor(220,220,220));

        QFont font("Segoe UI", 9, QFont::Bold);
        //...
        info->setFont(font);
        fingerprint->setFont(font);
        issuer->setFont(font);
        validity->setFont(font);
        subject->setFont(font);
        key->setFont(font);
        subjectAltNames->setFont(font);

        info->appendRow({new QStandardItem("Version"), info_verison});
        info->appendRow({new QStandardItem("signature Algorithm"), info_signatureAlgorithm});
        info->appendRow({new QStandardItem("Serial Number"), info_serialNumber});

        fingerprint->appendRow({new QStandardItem("md5"), fingerprint_md5});
        fingerprint->appendRow({new QStandardItem("sha1"), fingerprint_sha1});
        fingerprint->appendRow({new QStandardItem("sha256"), fingerprint_sha256});

        issuer->appendRow({new QStandardItem("Common Name"), issuer_commonName});
        issuer->appendRow({new QStandardItem("Organization Name"), issuer_organizationName});
        issuer->appendRow({new QStandardItem("Country Name"), issuer_countryName});
        issuer->appendRow({new QStandardItem("Locality Name"), issuer_localityName});
        issuer->appendRow({new QStandardItem("State Or Province"), issuer_stateOrProvinceName});
        issuer->appendRow({new QStandardItem("Email"), issuer_email});

        validity->appendRow({new QStandardItem("Not Before"), validity_notBefore});
        validity->appendRow({new QStandardItem("Not After"), validity_notAfter});

        subject->appendRow({new QStandardItem("Common Name"), subject_commonName});
        subject->appendRow({new QStandardItem("Organization Name"), subject_organizationName});
        subject->appendRow({new QStandardItem("Locality Name"), subject_localityName});
        subject->appendRow({new QStandardItem("State Or Province"), subject_stateOrProvinceName});
        subject->appendRow({new QStandardItem("Country Name"), subject_countryName});
        subject->appendRow({new QStandardItem("Email"), subject_email});

        key->appendRow({new QStandardItem("Type"), key_type});
        key->appendRow({new QStandardItem("Algorithm"), key_algorithm});

        /* the model */
        model->setColumnCount(2);
        model->setHorizontalHeaderLabels({"  Property", "  Value"});

        /* appending to the model */
        model->appendRow(info);
        model->appendRow(fingerprint);
        model->appendRow(validity);
        model->appendRow(issuer);
        model->appendRow(subject);
        model->appendRow(key);
        model->appendRow(subjectAltNames);
    }
    ~CertModel(){
        delete model;
    }
    // the model...
    QStandardItemModel *model;

private:
    /* ... */
    QStandardItem *info;
    QStandardItem *fingerprint;
    QStandardItem *issuer;
    QStandardItem *validity;
    QStandardItem *subject;
    QStandardItem *key;

public:
    QStandardItem *subjectAltNames;

    /* info */
    QStandardItem *info_verison;
    QStandardItem *info_signatureAlgorithm;
    QStandardItem *info_serialNumber;

    /* fingerprint */
    QStandardItem *fingerprint_md5;
    QStandardItem *fingerprint_sha1;
    QStandardItem *fingerprint_sha256;

    /* issuer */
    QStandardItem *issuer_commonName;
    QStandardItem *issuer_organizationName;
    QStandardItem *issuer_countryName;
    QStandardItem *issuer_localityName;
    QStandardItem *issuer_stateOrProvinceName;
    QStandardItem *issuer_email;

    /* validity */
    QStandardItem *validity_notBefore;
    QStandardItem *validity_notAfter;

    /* subject */
    QStandardItem *subject_commonName;
    QStandardItem *subject_organizationName;
    QStandardItem *subject_localityName;
    QStandardItem *subject_stateOrProvinceName;
    QStandardItem *subject_countryName;
    QStandardItem *subject_email;

    /* Key */
    QStandardItem *key_type;
    QStandardItem *key_algorithm;
};

#endif // CERTMODEL_H